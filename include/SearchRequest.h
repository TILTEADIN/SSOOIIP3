#pragma once
#include <string>
class SearchRequest {
    private:
        int id;
        std::string requestedWord;
    public:
        SearchRequest(int id, std::string requestedWord);
        ~SearchRequest();
        int getId();
        std::string getRequestedWord();
};