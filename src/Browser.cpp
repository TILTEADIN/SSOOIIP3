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


#include "../include/definitions.h"
#include "../src/Result.cpp"

class Browser {
    private:
        std::mutex *mtx;

    public:
        User *user;
        std::list<Result> result_list;

        Browser(std::mutex *mtx, User* user);
        ~Browser();

        int readFile(std::string path);
        void findWord(std::string each_line,int my_line);
        void skipText(std::ifstream& in_file,int task_begin,char delim);
        bool caseInsensitive(std::string each_word, std::string objective_word);
        void mainBrowser();
        void operator()();
};

Browser::Browser(std::mutex *mtx, User* user) {
    this->mtx = mtx;
    std::list<Result> result_list;
    this->user= user;
}

Browser::~Browser(){}

void Browser::operator()() {
	mainBrowser();
}

int numberFilesToRead() {
    DIR *dp;
    int numFiles = 0;
    struct dirent *ep;     
    dp = opendir (MATERIAL_PATH);

    if (dp != NULL){
        while ((ep = readdir (dp)))
            numFiles++;
        (void) closedir (dp);
    }else
        std::cout << BHIRED << " [BR] Couldn't open the directory" << BHIWHITE << std::endl;

    return numFiles;
}

/*  */
void Browser::mainBrowser() {
    std::string path = "prueba.txt";
    std::string completePath = MATERIAL_PATH + path;

    if(readFile(completePath) != 0){
        std::cout << BHIRED << " [BR] Error reading file" << BHIWHITE << std::endl; 
    }
}

/* Reads the file and checks if there's some error */
int Browser::readFile(std::string completePath) {
    std::ifstream inFile;
    std::string eachLine;
    int numLine = 0;

    inFile.open(completePath);

    if (!inFile) {
        std::cout << BHIRED << " [BR] Unable to open the file" << BHIWHITE << std::endl; 
        exit(1); // terminate with error
    }

    while (!inFile.eof()) {
        std::getline(inFile,eachLine);
        findWord(eachLine,numLine);
        numLine++;
    }

    inFile.close();
    return 0;
}

/* Method used for finding a word within a given string */
void Browser::findWord(std::string eachLine, int myLine){
	 
	std::string previousWord = "",word_next="";
	std::string objectiveWord = user->searchRequestQueue.front().getRequestedWord();
    user->searchRequestQueue.pop();
    std::string eachWord;  
    std::string str;

	std::istringstream strStream(eachLine);
	std::vector<std::string> lineVector;
    
    while (getline(strStream, str,' '))
		lineVector.push_back(str);

    for (unsigned int i = 0; i < lineVector.size(); i++) {
        eachWord = lineVector.at(i);
        
        if (caseInsensitive(eachWord,objectiveWord)){
            
            if (i == 0){
                previousWord = "";
                word_next = lineVector.at(i+1);

            } else if(i == lineVector.size()-1){
                word_next = "";
                previousWord = lineVector.at(i-1);

            } else{
                previousWord = lineVector.at(i-1);
                word_next = lineVector.at(i+1);
            }
            Result foundResult(previousWord, word_next, eachWord, (myLine+1));
            std::lock_guard<std::mutex> lock(*mtx);
            result_list.push_back(foundResult);
        }
	}
}

/* Checks whether a word is or isnt upper case and if so transforms it */
bool Browser::caseInsensitive(std::string each_word, std::string objective_word){
    bool check = false;
    unsigned int c_correct = 0;
    unsigned int len = each_word.size();
    
    for(unsigned int i = 0; i < len; i++){

        if((((each_word[i]) == (tolower(objective_word[i]))) || ((each_word[i]) == (toupper(objective_word[i]))))  && (check == false)){
            c_correct++;
        }else{
            if(ispunct(each_word[i])){  //Interesting method used to check if a character is ! , . ; and such
                c_correct++;
            }
            c_correct--;
        }
    }

    if(c_correct == objective_word.size()){
        check = true;
    }

    return check;
}
#endif