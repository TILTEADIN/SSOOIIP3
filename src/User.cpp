/******************************************************************
 * Project          : Práctica 3 de Sistemas Operativos II
 * Program name     : User.cpp
 * Authors          : Alberto Vázquez y Eduardo Eiroa
 * Date created     : 12/05/2021
 * Purpose          : Class that represents a user/client
 ******************************************************************/

#ifndef _USER_
#define _USER_

#include <queue>
#include <unistd.h>
#include <chrono>

#include "../src/SearchRequest.cpp"

#define MAX_FREE_RESULTS 5
#define MAXIMUM_CREDIT 15
#define FREE 1
#define LIMITED_PREMIUM 2
#define UNLIMITED_PREMIUM 3

//Each User will be treated with a correlation 1 User-> 1 Search Request once the Browser receives the array of Clients

class User {
	private:
		int id;
		int totalCredit;
		int currentCredit;
		int typeUser;
		bool served;
		std::string requestedWord;
		std::chrono::high_resolution_clock::time_point startTime;

	public:
		User(int id, int typeUser, std::string requestedWord,std::chrono::high_resolution_clock::time_point start);

		int getTotalCredit();
		int getCurrentCredit();
		int getId();
		int getTypeUser();
		int getServed();
		std::string getRequestedWord();
		std::chrono::high_resolution_clock::time_point getStartTime();
		void setCurrentCredit(int currentCredit);
		void setTotalCredit(int credit);
		void setServed(bool served);
		int generateRandomNumber(int max);
		
};

	User::User(int id ,int typeUser,std::string requestedWord,std::chrono::high_resolution_clock::time_point start){
		this->typeUser = typeUser;
		this->id = id;
		this->requestedWord = requestedWord;
		this->served = false;
		this->startTime= std::chrono::high_resolution_clock::now();

		switch (typeUser) {
			case FREE:
				this->totalCredit = MAX_FREE_RESULTS;
				break;
			case LIMITED_PREMIUM:
				this->totalCredit = generateRandomNumber(MAXIMUM_CREDIT);
				break;
			case UNLIMITED_PREMIUM:
				this->totalCredit = -1;
				break;
		}
		/*Total credits, initial and recharged ones
		Must be coinciding with results with some error margin,
		there can be some extra credits after recharging is done 
		and all the possible results have been found*/
		this->currentCredit = totalCredit; 
	}
	
	int User::getTotalCredit(){
		return totalCredit;
	}

	int User::getCurrentCredit(){
		return currentCredit;
	}

	int User::getId(){
		return id;
	}
	
	std::string User::getRequestedWord(){
		return requestedWord;
	}

	int User::getTypeUser(){
		return this->typeUser;}

	int User::getServed() {
		return this->served;
	}

	void User::setCurrentCredit(int currentCredit){
		this->currentCredit = currentCredit;
	}

	void User::setServed(bool served) {
		this->served = served;
	}

	void User::setTotalCredit(int credit) {
		this->totalCredit = credit;
	}

	std::chrono::high_resolution_clock::time_point  User::getStartTime() {
		return this->startTime;
	}

	int User::generateRandomNumber(int max){
		/*Had some trouble with the same number being generated no matter what the seed was;
		decided to put a sleep for 1 nano-second in order to keep changing the generated number.*/
    	usleep(1000);
    	struct timespec ts;
    	clock_gettime(CLOCK_MONOTONIC, &ts);
    	srand((time_t)ts.tv_nsec); /* using nano-seconds instead of seconds */
    	return ((rand() % max) + 1);
	}

#endif