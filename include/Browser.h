#pragma once

#include "../include/Thread_Structure.h"

#include <string>
#include <thread>
#include <mutex>
#include <shared_mutex>

class Browser
{
     private:
        Thread_Structure* thread_structure;
        std::mutex *mtx;
        int task_begin, task_end;

    public:
        Browser(Thread_Structure* thread_structure, std::mutex *mtx);
        ~Browser();
        int readFile(std::string path,int task_begin,int task_end);
        void findWord(std::string each_line,int my_line);
        void skipText(std::ifstream& in_file,int task_begin,char delim);
        bool caseInsensitive(std::string each_word, std::string objective_word);
        void main_Browser();
        void operator()();


};