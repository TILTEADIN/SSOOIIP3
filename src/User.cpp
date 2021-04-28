#include "../include/User.h"


//Cada usuario tiene asociada una peticion, de manera que nos ahorramos crear clases con poca responsabilidad
User::User(int id ,int initialCredit, bool isVip, std::string requestedWord){
    this->initialCredit = 0;
    this->currentCredit = initialCredit; //el saldo en el momento de la creación es el saldo incial
    this->id = id;
    this->isVip = isVip;
    this->requestedWord = requestedWord;
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

std::string User::getRequestedWord(){
    return requestedWord;
}

void User::setCurrentCredit(int currentCredit){
    this->currentCredit = currentCredit;
}