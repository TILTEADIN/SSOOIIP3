#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <string>

class Result{
    private:
        int line;
        std::string word_previous;
        std::string word_next;
        std::string word_objective;

    public:
        Result(std::string word_previous,std::string word_next,std::string word_objective,int line);
        ~Result();
        int getLine();
        std::string getWord_previous();
        std::string getWord_next();
        std::string getWord_objective();
};