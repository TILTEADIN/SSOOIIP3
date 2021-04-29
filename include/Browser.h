#pragma once
#include "../include/User.h"
#include "../include/Result.h"
#include <string>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <list>

class Browser {
    private:
        std::mutex *mtx;


    public:
        User *user;
        std::list<Result> result_list;
        Browser(std::mutex *mtx, User* user);
        ~Browser();
        int readFile(std::string path,int task_begin,int task_end);
        void findWord(std::string each_line,int my_line);
        void skipText(std::ifstream& in_file,int task_begin,char delim);
        bool caseInsensitive(std::string each_word, std::string objective_word);
        void main_Browser();
        void operator()();


};