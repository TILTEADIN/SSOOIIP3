#ifndef _RESULT_
#define _RESULT_
#include <iostream>
#include <fstream>
#include <thread>
#include <string>

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
};

/* Class used for storing the Results found along the texts */
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

#endif