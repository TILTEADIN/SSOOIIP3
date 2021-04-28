#include "../include/Browser.h"
#include "../include/Thread_Structure.h"
#include "../include/Result.h"

#include <ctype.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <iterator>
#include <sstream>
#include <algorithm>
using namespace std; 
  
Browser::Browser(Thread_Structure* thread_structure, mutex *mtx)
{	
    this->thread_structure = thread_structure; 
	task_begin = (*thread_structure).getThread_begin();
    task_end = (*thread_structure).getThread_end();
	this->mtx = mtx;
}

Browser::~Browser(){}

void Browser::operator()()
{
	main_Browser();
}

void Browser::main_Browser()
{
    string path = (*thread_structure).getFile_name();
    if(readFile(path, task_begin, task_end) != 0){
        printf("[Browser] Error reading file\n");
    }
    
}

//Reads the file and checks if there's some error
int Browser::readFile(string path,int task_begin,int task_end)
{
    ifstream in_file;
    string each_line;
    int my_line = task_begin;

    in_file.open(path);

    if (!in_file) {
        printf("[Error] Unable to open file\n");
        exit(1); // terminate with error
    }

    skipText(in_file,task_begin,NEW_LINE);

    while (!(in_file.eof()) && (task_end > my_line)) {
        getline(in_file,each_line);
        findWord(each_line,my_line);
        my_line++;
    }

    in_file.close();
    return 0;
}

//Method used for finding a word within a given string
void Browser::findWord(string each_line, int my_line){
 
	 
	string word_previous = "",word_next="";
	string objective_word = (*thread_structure).getWord();
    string each_word;
    string str;

	istringstream str_stream(each_line);
	vector<string> line_vector;
    
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
            lock_guard<mutex> lock(*mtx);
            (*thread_structure).pushResult_list(result_s);
        }
	}

}
//Checks whether a word is or isnt upper case and if so transforms it
bool Browser::caseInsensitive(string each_word,string objective_word){
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

//Skips text that we don't need to read 
void Browser::skipText(ifstream& in_file,int task_begin,char delim){
    int i = 0;
    while ( i++ < task_begin)
      in_file.ignore(MAX_LEN, delim); 

}