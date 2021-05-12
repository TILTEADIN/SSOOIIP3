/**********************************************************************
 * Project          : Práctica 3 de Sistemas Operativos II
 * Program name     : Result.cpp
 * Authors          : Alberto Vázquez y Eduardo Eiroa
 * Date created     : 12/05/2021
 * Purpose          : Class that represent a found result by searchers
 **********************************************************************/

#ifndef _RESULT_
#define _RESULT_

#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <sstream>

/* Class used for storing the Results found along the texts */
class Result{
    private:
        int line;
        std::string previousWord;
        std::string nextWord;
        std::string objectiveWord;
        std::string fileName;

    public:
        Result(std::string previousWord, std::string nextWord, std::string objectiveWord, int line, std::string fileName);
        ~Result();
        int getLine();
        std::string getPreviousWord();
        std::string getNextWord();
        std::string getObjectiveWord();
        std::string getFileName();
        void writeResultToFile(int userId);
};

Result::Result(std::string previousWord, std::string nextWord, std::string objectiveWord, int line, std::string fileName) {
    this->previousWord = previousWord;
    this->nextWord = nextWord;
    this->objectiveWord = objectiveWord;
    this->line = line;
    this->fileName = fileName;
}

Result::~Result(){}

std::string Result::getPreviousWord(){
    return this->previousWord;
}

std::string Result::getNextWord(){
    return this->nextWord;
}

std::string Result::getObjectiveWord(){
    return this->objectiveWord;
}

int Result::getLine() {
    return this->line;
}

std::string Result::getFileName() {
    return this->fileName;
}

/* Write the results of found words in one file for each user */
void Result::writeResultToFile(int userID){
    char path [256];
    sprintf (path, "./results/user%d.txt", userID); /* Name of file of each user results */
    std::ofstream file;
    std::stringstream auxiliaryFormatting;

    auxiliaryFormatting << " On file: " << fileName << 
                        " --> Matching Result at Line: " << line << 
                        " --> " << previousWord << " " << objectiveWord << 
                        " " <<nextWord<< std::endl;;

    std::string resultStr = auxiliaryFormatting.str();

    if (file.good()) { /* If file exists append*/
        file.open(path, std::ios_base::app);
    } else { /* If file doesn't exists create */
        file.open(path);
    }

    file << resultStr; /* Write on the file */

    file.close();
}
#endif