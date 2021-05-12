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

#include "../src/SearchRequest.cpp"

#define MAX_FREE_RESULTS 5
#define MAXIMUM_CREDIT 15
#define FREE 1
#define LIMITED_PREMIUM 2
#define UNLIMITED_PREMIUM 3

//Cada usuario tiene asociada una peticion, de manera que nos ahorramos crear clases con poca responsabilidad
class User {
	private:
		int id;
		int totalCredit;
		int currentCredit;
		int typeUser;
		bool served;
		std::string requestedWord;

	public:
		User(int id, int typeUser, std::string requestedWord);

		int getTotalCredit();
		int getCurrentCredit();
		int getId();
		int getTypeUser();
		int getServed();
		std::string getRequestedWord();
		void setCurrentCredit(int currentCredit);
		void setTotalCredit(int credit);
		void setServed(bool served);
		int generateRandomNumber(int max);
};

	User::User(int id ,int typeUser,std::string requestedWord){
		this->typeUser = typeUser;
		this->id = id;
		this->requestedWord = requestedWord;
		this->served = false;
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
		this->currentCredit = totalCredit; /* Total credits, initial and recharged ones */
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

	int User::generateRandomNumber(int max){
    	usleep(1000);
    	struct timespec ts;
    	clock_gettime(CLOCK_MONOTONIC, &ts);
    	srand((time_t)ts.tv_nsec); /* using nano-seconds instead of seconds */
    	return ((rand() % max) + 1);
	}

#endif