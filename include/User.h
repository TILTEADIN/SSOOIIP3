#include <string>

class User {
    private:
        int id;
        int initialCredit;
        int currentCredit;
        bool isVip;
        std::string requestedWord;

    public:
        User(int id, int initialCredit, bool isVip, std::string requestedWord);

        int getInitialCredit();
        int getCurrentCredit();
        int getId();
        bool getIsVip();
        std::string getRequestedWord();

        void setCurrentCredit(int currentCredit);
};