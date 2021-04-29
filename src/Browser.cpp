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

#include "../include/Browser.h"
#include "../include/Result.h"
#include "../include/definitions.h"

Browser::Browser(std::mutex *mtx, User* user)
{
    this->mtx = mtx;
    std::list<Result> result_list;
    this->user= user;
}

Browser::~Browser(){}

void Browser::operator()()
{
	main_Browser();
}

int numberFilesToRead() {
    DIR *dp;
    int numFiles = 0;
    struct dirent *ep;     
    dp = opendir ("../material");

    if (dp != NULL){
        while ((ep = readdir (dp)))
            numFiles++;
        (void) closedir (dp);
    }else
        std::cout << BHIRED << " [BR] Couldn't open the directory" << BHIWHITE << std::endl;

    return numFiles;
}

/*  */
void Browser::main_Browser()
{
    //for (int i = 0; i < numberFilesToRead(); i++) {
    //}

    if(readFile(path) != 0){
        std::cout << BHIRED << " [BR] Error reading file" << BHIWHITE << std::endl; 
    }
}

/* Reads the file and checks if there's some error */
int Browser::readFile(std::string path,int task_begin,int task_end)
{
    std::ifstream in_file;
    std::string each_line;
    int my_line = task_begin;

    in_file.open(path);

    if (!in_file) {
        std::cout << BHIRED << " [BR] Unable to open the file" << BHIWHITE << std::endl; 
        exit(1); // terminate with error
    }

    while (!(in_file.eof()) && (task_end > my_line)) {
        getline(in_file,each_line);
        findWord(each_line,my_line);
        my_line++;
    }

    in_file.close();
    return 0;
}

/* Method used for finding a word within a given string */
void Browser::findWord(std::string each_line, int my_line){
	 
	std::string word_previous = "",word_next="";
	std::string objective_word = user->getRequestedWord();
    std::string each_word;  
    std::string str;

	std::istringstream str_stream(each_line);
	std::vector<std::string> line_vector;
    
    while (getline(str_stream, str,' '))
		line_vector.push_back(str);

    for (unsigned int i = 0; i < line_vector.size(); i++) {
        each_word = line_vector.at(i);
        
        if(caseInsensitive(each_word,objective_word)){
            
            if(i == 0){
                word_previous = "";
                word_next = line_vector.at(i+1);

            }else if(i == line_vector.size()-1){
                word_next = "";
                word_previous = line_vector.at(i-1);

            }else{
                word_previous = line_vector.at(i-1);
                word_next = line_vector.at(i+1);
            }
            Result result_s(word_previous, word_next, each_word, (my_line+1));
            std::lock_guard<std::mutex> lock(*mtx);
            result_list.push_back(result_s);
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
