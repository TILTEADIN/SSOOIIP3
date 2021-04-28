#pragma once

#include "../include/Result.h"
#include "../include/definitions.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <list>
#include <string>

using namespace std;

class Thread_Structure{

private:
    int thread_begin;
    int thread_end;
    int n_thread;
    string word;
    string file_name;
    list <Result> result_list;

public:
    Thread_Structure(int thread_begin, int thread_end,string word,string file_name,int n_thread);
    ~Thread_Structure();
    int getThread_begin();
    int getThread_end();
    string getWord();
    string getFile_name();
    void pushResult_list(Result result_s);
    void toString();

};