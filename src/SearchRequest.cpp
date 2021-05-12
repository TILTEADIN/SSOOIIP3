/*********************************************************************
 * Project          : Práctica 3 de Sistemas Operativos II
 * Program name     : SearchRequest.cpp
 * Authors          : Alberto Vázquez y Eduardo Eiroa
 * Date created     : 12/05/2021
 * Purpose          : Class that represent a search request of a word
 *********************************************************************/

#ifndef _SEARCH_REQUEST_
#define _SEARCH_REQUEST_
#include <string>

class SearchRequest {
    private:
        int id;
        std::string requestedWord;
    public:
        SearchRequest(int id, std::string requestedWord);
        int getId();
        std::string getRequestedWord();
};

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
#endif