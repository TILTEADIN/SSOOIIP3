#pragma once
#include <string>
#include <queue>
#include "../include/SearchRequest.h"

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