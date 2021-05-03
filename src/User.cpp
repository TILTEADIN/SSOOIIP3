#ifndef _USER_
#define _USER_
#include <queue>
#include "../src/SearchRequest.cpp"
//Cada usuario tiene asociada una peticion, de manera que nos ahorramos crear clases con poca responsabilidad

class User {
	private:
		int id;
		int initialCredit;
		int currentCredit;
		bool isVip;
		//std::string requestedWord;

	public:
		std::queue<SearchRequest> searchRequestQueue;
		User(int id, int initialCredit, bool isVip, std::queue<SearchRequest> searchRequestQueue);

		int getInitialCredit();
		int getCurrentCredit();
		int getId();
		bool getIsVip();
		//std::string getRequestedWord();
		void setCurrentCredit(int currentCredit);
};

	User::User(int id ,int initialCredit, bool isVip,std::queue<SearchRequest> searchRequestQueue){
		this->initialCredit = 0;
		this->currentCredit = initialCredit; //el saldo en el momento de la creación es el saldo incial
		this->id = id;
		this->isVip = isVip;
		this->searchRequestQueue = searchRequestQueue;
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

	bool User::getIsVip(){
		return isVip;
	}
	/*
	std::string User::getRequestedWord(){
		return requestedWord;
	}*/

	void User::setCurrentCredit(int currentCredit){
		this->currentCredit = currentCredit;
	}

#endif