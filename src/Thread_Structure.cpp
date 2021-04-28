#include "../include/Thread_Structure.h"
#include "../include/Result.h"
#include <definitions.h>
#include <list>

using namespace std;

//Thread begin and end stored in every object of this class to avoid repetitiviness 
Thread_Structure::Thread_Structure(int thread_begin, int thread_end,string word,string file_name,int n_thread){
       
        this->thread_begin=thread_begin;
        this->thread_end=thread_end;
        this->word=word;
        this->file_name=file_name;

        this->n_thread = n_thread;

        result_list = list <Result> ();

}

Thread_Structure::~Thread_Structure(){}

//Pushes back to the end of the list the Result received
void Thread_Structure::pushResult_list(Result result_s){
        result_list.push_back(result_s);
}

//Using a list to store results an later print them through this function
void Thread_Structure::toString(){
        string str_Result;
        while (!result_list.empty())
	{       
		string Result_string = ("[Thread "+to_string(n_thread)+" starting at:"+to_string(thread_begin)+" – ends at: "+to_string(thread_end)+"] :: line "+ to_string(result_list.front().getLine()) +" :: … "+ result_list.front().getWord_previous() +" "+ result_list.front().getWord_objective() +" "+ result_list.front().getWord_next() +" … \r\t");
                printf("%s\n",Result_string.c_str());
                result_list.pop_front();
	}
}

int Thread_Structure::getThread_begin(){return thread_begin;}
int Thread_Structure::getThread_end(){return thread_end;}
string Thread_Structure::getWord(){return word;}
string Thread_Structure::getFile_name(){return file_name;}
