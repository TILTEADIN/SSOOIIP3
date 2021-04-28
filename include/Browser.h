#pragma once

#include "../include/Thread_Structure.h"

#include <string>
#include <thread>
#include <mutex>
#include <shared_mutex>

using namespace std;

class Browser
{
     private:
        Thread_Structure* thread_structure;
        mutex *mtx;
        int task_begin, task_end;

    public:
        Browser(Thread_Structure* thread_structure, mutex *mtx);
        ~Browser();
        int readFile(string path,int task_begin,int task_end);
        void findWord(string each_line,int my_line);
        void skipText(ifstream& in_file,int task_begin,char delim);
        bool caseInsensitive(string each_word,string objective_word);
        void main_Browser();
        void operator()();


};