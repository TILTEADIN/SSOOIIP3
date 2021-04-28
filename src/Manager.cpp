#include <chrono>
#include <algorithm>
#include <functional>
#include <vector>

#include "PaymentGateway.cpp"

std::vector<std::string> diccionary = {"hola","eduardo"};

/* Request to payment service to recharge credit of a given user */
void requestCreditRecharge(User *usuario) {
    rechargeCreditRequestMutex.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << BHIGREEN << " [MG] El usuario " << usuario->getId() << 
                " solicita una recarga de créditos " << BHIWHITE << std::endl;

    rechargeCreditRequestQueue.push(usuario);
    cv.notify_one();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << BHIGREEN << " [MG] Saldo del usuario " << usuario->getId() << " recargado a " 
        << usuario->getCurrentCredit() << " créditos." << BHIWHITE << std::endl;
}    

/* Select a random word for the diccionary */
std::string selectRandomWord() {
    int size = diccionary.size();

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
        clients.push_back(User(i, generateRandomNumber(MAXIMUM_CREDIT), generateIsVip(),"palabra"));
        threads.push_back(std::thread(requestCreditRecharge, &clients[i]));
    }

    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    std::cout << "fin";
        
    pgThread.join();
    
    return 0;
}   