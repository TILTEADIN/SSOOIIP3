#include "../include/SearchRequest.h"

SearchRequest::SearchRequest(int id ,std::string requestedWord){
    this->id = id;
    this->requestedWord = requestedWord;
}

std::string SearchRequest::getRequestedWord(){
    return this->requestedWord;
}

int SearchRequest::getId() {
    return this->id;
}