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
        Result(std::string previousWord, std::string nextWord, std::string objectiveWord, int line);
        ~Result();
        int getLine();
        std::string getPreviousWord();
        std::string getNextWord();
        std::string getObjectiveWord();
};