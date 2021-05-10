#include <string>
#include <queue>

#include "../include/SearchRequest.h"

//If client is vip variable isVIP will be set to 1 and credit to -1.
//If its a VIP Client with credit isVIP will be set to one and intital and actual credit will be updated to the needed value
//If the client is a free one, the credit will be set to -1 and isVIP to 0.

class User {
    private:
        int id;
        int initialCredit;
        int currentCredit;
        //std::string requestedWord;

    public:
        std::queue<SearchRequest> searchRequestQueue;
        User(int id, int initialCredit,std::queue<SearchRequest> searchRequestQueue);

        int getInitialCredit();
        int getCurrentCredit();
        int getId();
        //std::string getRequestedWord();
        void setCurrentCredit(int currentCredit);
};