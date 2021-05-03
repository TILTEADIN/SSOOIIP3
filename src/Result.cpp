#include <iostream>
#include <fstream>
#include <thread>
#include <string>

#include "../include/Result.h"
#include "../include/definitions.h"

/* Class used for storing the Results found along the texts */
Result::Result(std::string word_previous, std::string word_next, std::string word_objective, int line){
	this->word_previous = word_previous;
	this->word_next = word_next;
        this->word_objective = word_objective;
        this->line = line;
}

Result::~Result(){}

std::string Result::getWord_previous(){return word_previous;}
std::string Result::getWord_next(){return word_next;}
std::string Result::getWord_objective(){return word_objective;}
int Result::getLine(){return line;}
