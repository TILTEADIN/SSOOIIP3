#include <string>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <list>


#ifndef _BROWSER_
#define _BROWSER_
#include "../include/User.h"
#include "../include/Result.h"

class Browser {
    private:
        std::mutex *mtx;

    public:
        User *user;
        std::list<Result> result_list;

        Browser(std::mutex *mtx, User* user);
        ~Browser();

        int readFile(std::string path);
        void findWord(std::string each_line,int my_line);
        void skipText(std::ifstream& in_file,int task_begin,char delim);
        bool caseInsensitive(std::string each_word, std::string objective_word);
        void mainBrowser();
        void operator()();
};
#endif