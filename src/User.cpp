#ifndef _USER_
#define _USER_
#include <queue>

#include "../src/SearchRequest.cpp"
#include <unistd.h>

#define FREE 0
#define LIMITED_VIP 1
#define UNLIMITED_VIP 2
#define MAX_FREE_RESULTS 5
#define MAXIMUM_CREDIT 15

//Cada usuario tiene asociada una peticion, de manera que nos ahorramos crear clases con poca responsabilidad

class User {
	private:
		int id;
		int totalCredit;
		int currentCredit;
		int typeUser;
		bool served;
		std::string requestedWord;
		//std::string requestedWord;

	public:
		//std::queue<SearchRequest> searchRequestQueue;
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
			case 1:
				this->totalCredit = MAX_FREE_RESULTS;
				break;
			case 2:
				this->totalCredit = generateRandomNumber(MAXIMUM_CREDIT);
				break;
			case 3:
				this->totalCredit = -1;
				break;
		}
		//this->requestedWord = searchRequestQueue.front().getRequestedWord();
		//searchRequestQueue.pop();
		this->currentCredit = totalCredit; //el saldo en el momento de la creación es el saldo incial
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