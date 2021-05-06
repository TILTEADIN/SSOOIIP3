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
		int initialCredit;
		int currentCredit;
		int typeUser;
		std::string requestedWord;
		//std::string requestedWord;

	public:
		//std::queue<SearchRequest> searchRequestQueue;
		User(int id, int typeUser, std::string requestedWord);

		int getInitialCredit();
		int getCurrentCredit();
		int getId();
		int getTypeUser();
		std::string getRequestedWord();
		void setCurrentCredit(int currentCredit);
		int generateRandomNumber(int max);
};

	User::User(int id ,int typeUser,std::string requestedWord){
		this->typeUser = typeUser;
		this->currentCredit = initialCredit; //el saldo en el momento de la creación es el saldo incial
		this->id = id;
		this->requestedWord = requestedWord;
		switch (typeUser) {
			case 0:
				this->initialCredit = MAX_FREE_RESULTS;
				break;
			case 1:
				this->initialCredit = generateRandomNumber(MAXIMUM_CREDIT);
				break;
			case 2:
				this->initialCredit = -1;
				break;
		}
		//this->requestedWord = searchRequestQueue.front().getRequestedWord();
		//searchRequestQueue.pop();
	}

	int User::getInitialCredit(){
		return initialCredit;
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

	void User::setCurrentCredit(int currentCredit){
		this->currentCredit = currentCredit;
	}

	int User::generateRandomNumber(int max){
		usleep(3000);
		srand((unsigned) time(0));
		return ((rand() % max) + 1);
	}

#endif