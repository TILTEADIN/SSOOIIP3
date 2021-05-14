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