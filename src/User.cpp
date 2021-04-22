#include <iostream>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <string>

#include "../include/User.h"

int generateCredit(){
  srand((unsigned) time(0));
  return ((rand() % 15) + 1);
}

//Cada usuario tiene asociada una peticion, de manera que nos ahorramos crear clases con poca responsabilidad
User::User(int initialCredit, int currentCredit, int id, bool isVip,std::string requestedWord){
        
	  this-> initialCredit=generateCredit();
    this-> currentCredit=initialCredit; //el saldo en el momento de la creación es el saldo incial
	  this-> id=id;
    this-> isVip=isVip;
    this-> requestedWord=requestedWord;
}

int User::getInitialCredit(){return initialCredit;}
int User::getCurrentCredit(){return currentCredit;}
int User::getId(){return id;}
bool User::getIsVip(){return isVip;}
std::string User::getRequestedWord(){return requestedWord;}

void User::setCurrentCredit(int currentCredit){
    this->currentCredit = currentCredit;
}