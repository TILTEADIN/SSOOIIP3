#ifndef MANAGER
#define MANAGER
#include "../src/PaymentGateway.cpp"
#include "../src/Browser.cpp"


std::vector<std::string> diccionary = {"cuadro","presidente","vendendores","titulo","precio","castillo"};

//ESTE METODO TIENE QUE ESTAR EN BROWSER o en definitions
/* Request to payment service to recharge credit of a given user */
void requestCreditRecharge(User *user) {
    rechargeCreditRequestMutex.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << BHIGREEN << " [MG] User n: " << user->getId() << 
                " requests a credit top up " << BHIWHITE << std::endl;

    rechargeCreditRequestQueue.push(user);
    cv.notify_one();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << BHIGREEN << " [MG] User's credit" << user->getId() << " topped up to " 
        << user->getCurrentCredit() << " credits." << BHIWHITE << std::endl;
}    

/* Select a random word for the diccionary */
std::string selectRandomWord() {
    int num = generateRandomNumber(diccionary.size());
    return diccionary[num];
}

/* Generate if user is vip or not */
bool generateIsVip() {
    bool isVip = false;

    if (generateRandomNumber(1) == 1) {
        isVip = true;
    }
    return isVip;
}

/* Main function */
int main(int argc, char *argv[]) {

    /* Launch payment service */
    PaymentGateway pg;
    std::thread pgThread(pg);
    std::vector<User> clients;
    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_CLIENTS; i++) {
        std::queue<SearchRequest> searchRequestQueue;
        searchRequestQueue.push(SearchRequest(i,selectRandomWord()));
        clients.push_back(User(i, generateRandomNumber(MAXIMUM_CREDIT), generateIsVip(), searchRequestQueue));
        std::mutex sem;
        threads.push_back(std::thread(Browser(&sem, &clients[i])));
    }

    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    end=true;

    return 0;
}   
#endif