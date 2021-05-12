/******************************************************************
 * Project          : Práctica 3 de Sistemas Operativos II
 * Program name     : Browser.cpp
 * Authors          : Alberto Vázquez y Eduardo Eiroa
 * Date created     : 12/05/2021
 * Purpose          : Class that launch child searchers in order
 *                    to find words associated with users
 ******************************************************************/

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

    public:
        User *user;
        std::vector<Result> result_list;

        Browser(std::mutex *mtx, User* user);
        ~Browser();

        int readFile(std::string completePath, std::string fileName);
        void findWord(std::string eachLine, int myLine, std::string fileName);
        bool caseInsensitive(std::string eachWord, std::string objectiveWord);
        void manageUserCredit();
        void requestCreditRecharge();
        void mainBrowser();
        bool checkCredit();
        void generateResults();
        void launchSearchers(std::vector<std::thread> &searchers, std::vector<std::string> filesNames, int numFiles);
        void operator()();
};

Browser::Browser(std::mutex *mtx, User* user) {
    this->mtx = mtx;
    this->user= user;
    std::vector<Result> result_list;
}

Browser::~Browser(){}

void Browser::operator()() {
    mainBrowser();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
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

/* Launch as many threads as files in material directory */
void Browser::launchSearchers(std::vector<std::thread> &searchers, std::vector<std::string> filesNames, int numFiles) {
    for (int i = 0; i < numFiles; i++) {
        std::string fileName = filesNames[i];
        std::string completePath = MATERIAL_PATH + fileName;
        searchers.push_back(std::thread([completePath, fileName, this] {readFile(completePath, fileName);}));
    }

    std::for_each(searchers.begin(), searchers.end(), std::mem_fn(&std::thread::join));
}

/* Write results for each users file results */
void Browser::generateResults() {
    if (result_list.size() != 0) {
        for (int i = 0; i < result_list.size(); i++) {
            //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            result_list[i].writeResultToFile(user->getId());
        }
    } else {
        std::cout << BHIRED << " [BR] No results found for the user " << user->getId() << BHIWHITE << std::endl; 
    }
}

/* Launch the browser children for each file in material directory */
void Browser::mainBrowser() {
    std::vector<std::string> filesNames;
    std::vector<std::thread> searchers;

    int numFiles = numberFilesToRead(filesNames);

    std::cout << BHIYELLOW << " [BG] Search word for user " << user->getId() << 
                            ": '" << user->getRequestedWord() << "'" << BHIWHITE << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    launchSearchers(searchers, filesNames, numFiles);

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    /* Pop a search request element */
    searchRequestQueueMutex.lock();
    searchRequestQueue.pop();
    searchRequestQueueMutex.unlock();

    /* Notify that the search request queue has changed */
    searchRequestCV.notify_one();

    /* Write found results on files */
    generateResults();
}

/* Reads the file and checks if there's some error */
int Browser::readFile(std::string completePath, std::string fileName) {
    std::ifstream inFile;
    std::string eachLine;
    int numLine = 0;

    try {
        inFile.open(completePath);

        while (!inFile.eof()) {
            std::getline(inFile,eachLine);
            findWord(eachLine,numLine,fileName);
            numLine++;
        }

        inFile.close();

    } catch (std::exception &e) {
        std::cout << BHIRED << " [BR] An exception ocurred: " << e.what() << std::endl;
    }

    /*
    if (!inFile) {
        std::cout << BHIRED << " [BR] Unable to open the file" << BHIWHITE << std::endl; 
        std::exit(EXIT_FAILURE);
    }*/

    return 0;
}

/* Request to payment service to recharge credit of a given user */
void Browser::requestCreditRecharge() {
    //rechargeCreditRequestMutex.lock();
    TopUpRequest request(user);

    rechargeCreditRequestMutex.lock();
    rechargeCreditRequestQueue.push(std::move(request));
    rechargeCreditRequestMutex.unlock();
    
    paymentGatewayCV.notify_one();
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << BHIYELLOW << " [BG] User " << user->getId() << 
                " requests a credit recharge" << BHIWHITE << std::endl;

    //std::cout << "hola " << rechargeCreditRequestQueue.size() << std::endl;
    rechargeCreditRequestMutex.lock();
    int credit = rechargeCreditRequestQueue.front().clientRequestFuture.get();
    rechargeCreditRequestMutex.unlock();

    //std::cout << "adios" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    std::cout << BHIYELLOW << " [BG] User's credit " << user->getId() << " recharge to " 
        << user->getCurrentCredit() << " credits. (total: " << user->getTotalCredit() 
        << ")" << BHIWHITE << std::endl;
}   

/* Decrease credit of free and premium limited users */
void Browser::manageUserCredit(){
   if(user->getTypeUser()== 1 || user->getTypeUser()==2){
       semUserCredit.lock();
       user->setCurrentCredit(user->getCurrentCredit()-1);
       semUserCredit.unlock();
   }
}

/* Check if users credit is zero */
bool Browser::checkCredit() {

    bool enoughCredit = false; 

    semUserCredit.lock();
    switch (user->getTypeUser()) {
        case 1:
            if (user->getCurrentCredit() > 0)
                enoughCredit = true;
            break;
        case 2:
            if (user->getCurrentCredit() == 0)
                requestCreditRecharge();
            enoughCredit = true;
            break;
        case 3:
            enoughCredit = true;
            break;
    }
    semUserCredit.unlock();

    return enoughCredit;
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
        
    for (unsigned int i = 0; i < lineVector.size() && checkCredit(); i++) {
        eachWord = lineVector.at(i);
        
        if (caseInsensitive(eachWord,user->getRequestedWord())){
            
            if (i == 0 && i == lineVector.size()-1) {
                previousWord = "";
                nextWord = "";
            } else if (i == 0) {
                previousWord = "";
                nextWord = lineVector.at(i+1);
            } else if (i == lineVector.size()-1) {
                previousWord = lineVector.at(i-1);
                nextWord = "";
            } else {
                previousWord = lineVector.at(i-1);
                nextWord = lineVector.at(i+1);
            }

            Result foundResult(previousWord, nextWord, eachWord, (myLine+1), fileName);
            manageUserCredit();
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