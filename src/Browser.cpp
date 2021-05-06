#ifndef _BROWSER_
#define _BROWSER_

#include <ctype.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <list>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <queue>

#include "../include/definitions.h"
#include "../src/Result.cpp"

class Browser {
    private:
        std::mutex *mtx;
        std::string objectiveWord;

    public:
        User *user;
        std::vector<Result> result_list;

        Browser(std::mutex *mtx, User* user);
        ~Browser();

        int readFile(std::string completePath, std::string fileName);
        void findWord(std::string eachLine, int myLine, std::string fileName);
        bool caseInsensitive(std::string eachWord, std::string objectiveWord);
        void mainBrowser();
        void operator()();
};

Browser::Browser(std::mutex *mtx, User* user) {
    this->mtx = mtx;
    this->user= user;
    this->objectiveWord = user->getRequestedWord();
    std::vector<Result> result_list;
    //user->searchRequestQueue.pop();
}

Browser::~Browser(){}

/* Returns the type of user */
//int getTypeOfUser() {

//}

void Browser::operator()() {
    mainBrowser();
}

/* Returns the number of files on the material directory */
int numberFilesToRead(std::vector<std::string> &filesNames) {
    DIR *dp;
    int numFiles = 0;
    struct dirent *ep;
    dp = opendir (MATERIAL_PATH);

    if (dp != NULL){
        while ((ep = readdir (dp))) {
            /* Don't include . and .. directories */
            if (strcmp(ep->d_name, ".") && strcmp(ep->d_name, "..")) { 
                filesNames.push_back(ep->d_name);
                numFiles++;
            }
        }
        (void) closedir (dp);
    }else
        std::cout << BHIRED << " [BR] Couldn't open the directory" << BHIWHITE << std::endl;

    return numFiles; 
}

void addSearchRequest(User *user, std::string objectiveWord) {
    std::unique_lock<std::mutex> ul(searchRequestMutex);
    searchRequestCV.wait(ul,[] {return (searchQueue.size() < N_SEARCH_MAX);});
    
    SearchRequest rq(user->getId(),objectiveWord);
    searchQueue.push(rq);
    std::cout << BHIYELLOW << " [BR] La cola de peticiones tiene " << 
            (N_SEARCH_MAX - searchQueue.size()) << " huecos disponibles" << 
            " (peticion del usuario " << user->getId() << ")" << BHIWHITE << std::endl;
    ul.unlock();
}

void removeSearchRequest(User *user, std::string objectiveWord) {
    searchRequestQueueMutex.lock();
    searchQueue.pop();
    std::cout << BHIYELLOW << " [BR] Búsqueda de la palabra '" << objectiveWord << 
            "' (Usuario " <<  user->getId() << ") a finalizado" << BHIWHITE << std::endl;
    searchRequestCV.notify_one();
    searchRequestQueueMutex.unlock();
}

/* Launch the browser children for each file in material directory */
void Browser::mainBrowser() {
    std::vector<std::string> filesNames;
    std::vector<std::thread> searchers;

    int numFiles = numberFilesToRead(filesNames);

    std::cout << BHIYELLOW << " [BG] Palabra a buscar para el usuario " << user->getId() <<
                             ": '" << objectiveWord << "'" << BHIWHITE << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

   
    

    

   // addSearchRequest(this->user, this->objectiveWord);

    /* Launch as many threads as files in material directory */
    for (int i = 0; i < numFiles; i++) {
        std::string fileName = filesNames[i];
        std::string completePath = MATERIAL_PATH + fileName;        
        searchers.push_back(std::thread([completePath, fileName, this] {readFile(completePath, fileName);}));
    }

    std::for_each(searchers.begin(), searchers.end(), std::mem_fn(&std::thread::join));

    //removeSearchRequest(this->user, this->objectiveWord);

    semConcurrentBrowser.signal();


    //aqui utilizar promise and uture para sincronizar todoos los hijos
    //RECORDAR: utilizar promise y future para propagar las excepciones 
    //Aqui escribirá en un archivo para cada usuario.
    if (result_list.size() != 0) {
        for (int i = 0; i < result_list.size(); i++) {
            std::cout << BHIPURPLE << " [BR] [RESULTS] Usuario: " << user->getId() << " - archivo: " << result_list[i].getFileName() << " - " 
                << "linea: " << result_list[i].getLine() << ": " 
                << result_list[i].getPreviousWord() << " " 
                << result_list[i].getObjectiveWord() << " "
                << result_list[i].getNextWord() << BHIWHITE << std::endl;
        }
    } else {
        std::cout << BHIRED << " [BR] No se ha encontrado resultados" << BHIWHITE << std::endl; 
    }
}

/* Reads the file and checks if there's some error */
int Browser::readFile(std::string completePath, std::string fileName) {
    std::ifstream inFile;
    std::string eachLine;
    int numLine = 0;

    inFile.open(completePath);

    if (!inFile) {
        std::cout << BHIRED << " [BR] Unable to open the file" << BHIWHITE << std::endl; 
        std::exit(EXIT_FAILURE); /* terminate with error */
    }

    while (!inFile.eof()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::getline(inFile,eachLine);
        findWord(eachLine,numLine,fileName);
        numLine++;
    }
    
    inFile.close();

    return 0;
}

/* Method used for finding a word within a given string */
void Browser::findWord(std::string eachLine, int myLine, std::string fileName){
	 
	std::string previousWord = "",nextWord = "";
    std::string eachWord;  
    std::string str;

	std::istringstream strStream(eachLine);
	std::vector<std::string> lineVector;
    
    while (getline(strStream, str,' '))
		lineVector.push_back(str);;
        
    for (unsigned int i = 0; i < lineVector.size(); i++) {
        eachWord = lineVector.at(i);
        
        if (caseInsensitive(eachWord,this->objectiveWord)){
            
            if (i == 0){
                previousWord = "";
                nextWord = lineVector.at(i+1);

            } else if(i == lineVector.size()-1){
                nextWord = "";
                previousWord = lineVector.at(i-1);

            } else{
                previousWord = lineVector.at(i-1);
                nextWord = lineVector.at(i+1);
            }
            Result foundResult(previousWord, nextWord, eachWord, (myLine+1), fileName);
            std::lock_guard<std::mutex> lock(*mtx);
            result_list.push_back(foundResult);
        }
	}
}

/* Checks whether a word is or isnt upper case and if so transforms it */
bool Browser::caseInsensitive(std::string eachWord, std::string objectiveWord){
    bool check = false;
    unsigned int cCorrect = 0;
    unsigned int len = eachWord.size();
    
    for (unsigned int i = 0; i < len; i++) {
        if ((((eachWord[i]) == (tolower(objectiveWord[i]))) || 
            ((eachWord[i]) == (toupper(objectiveWord[i])))) && 
            (check == false)) {
            cCorrect++;
        } else {
            /* Interesting method used to check if a character is ! , . ; and such */
            if (ispunct(eachWord[i])) {  
                cCorrect++;
            }
            cCorrect--;
        }
    }

    if(cCorrect == objectiveWord.size()){
        check = true;
    }

    return check;
}
#endif