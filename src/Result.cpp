#include "../include/Result.h"
#include "../include/definitions.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <string>

using namespace std;
//Class used for storing the Results found along the texts

Result::Result(string word_previous,string word_next,string word_objective,int line){
        
	this->word_previous = word_previous;
	this->word_next = word_next;
        this->word_objective = word_objective;
        
        this->line = line;
        
}

Result::~Result(){}

string Result::getWord_previous(){return word_previous;}
string Result::getWord_next(){return word_next;}
string Result::getWord_objective(){return word_objective;}
int Result::getLine(){return line;}
