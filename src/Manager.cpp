/******************************************************************
 * Project          : Práctica 3 de Sistemas Operativos II
 * Program name     : Manager.cpp
 * Authors          : Alberto Vázquez y Eduardo Eiroa
 * Date created     : 12/05/2021
 * Purpose          : Create users and launch thread for each user
 ******************************************************************/

#ifndef _MANAGER_
#define _MANAGER_

#include <csignal>

#include "../src/PaymentGateway.cpp"
#include "../src/Browser.cpp"

std::vector<std::string> diccionary = {"prueba","cuadro","tenue","vendedores",
                                    "titulo","acantilados","castillo","brujula","beneficios",
                                    "internet","complacido","pretender","confiabilidad",
                                    "profano","sorpresa","comprension","admirador"};

/* Select a random word for the diccionary */
std::string selectRandomWord() {
    int num = generateRandomNumber(diccionary.size())-1;
    std::string word = diccionary[num];
    diccionary.erase(diccionary.begin()+num); /* Delete the chosen word from diccionary */
    return word;
}

/* Generate if user is vip or not */
bool generateIsVip() {
    bool isVip = false;

    if (generateRandomNumber(1) == 1) {
        isVip = true;
    }
    return isVip;
}

/* Signal handler */
void signalHandler(int signum) {
    std::cout << BHIGREEN << "\n [MG] Exit program (Ctrl+C)..." << BHIWHITE << std::endl;
    std::exit(EXIT_SUCCESS);
}

/* Install signal handler */
void installSignalHandler() {
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        std::cerr << BHIRED << " [MG] Error installing signal handler" << BHIWHITE << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

/* Returns a client index that better matchs with client preference */
int searchForClientType(int clientPreference, std::vector<User> clients) {
    int index = -1; /* Returns -1 if there is no user without being attended */

    for (int i = 0; i < clients.size(); i++) {
        if (!clients[i].getServed()) {
            if (clientPreference > 8){
                if(clients[i].getTypeUser() == FREE) {
                    index = i;
                    break;
                }
            } else {
                if(clients[i].getTypeUser() == LIMITED_PREMIUM || clients[i].getTypeUser() == UNLIMITED_PREMIUM){
                    index = i;
                    break;
                }
            } 
        }
    }
    return index;
}

/* Returns the type of user name */
std::string getNameTypeUser(int typeUser) {
    std::string nameTypeUser;

    switch(typeUser){
        case FREE:
            nameTypeUser = "free";
            break;
        case LIMITED_PREMIUM:
            nameTypeUser = "limited premimum";
            break;
        case UNLIMITED_PREMIUM:
            nameTypeUser = "unlimited premimum";
            break;
    }
    return nameTypeUser;
}

/* Check if all clients are served to finish the main loop */
bool allClientServed(std::vector<User> clients) {
    int cnt = 0;
    bool go = true;

    for (int i = 0; i < clients.size(); i++) {
        if (clients[i].getServed() == true) {
            cnt++;
        }
    }

    if (cnt == clients.size()) {
        go = false;
    }
    return go;
}

/* Create the user's vector */
void createUsers(std::vector<User> &clients) {
    for (int i = 1; i <= NUM_CLIENTS; i++) {
        /* Number three is passed as argument for generateRandomNumber so one of the three possible types of user is generated */
        clients.push_back(User(i, generateRandomNumber(3), selectRandomWord()));
    }
}

/* Print info for each user */
void printUserServedInfo(std::vector<User> clients, int index) {

    if (clients[index].getTypeUser() == FREE || clients[index].getTypeUser() == LIMITED_PREMIUM) {
        std::cout << BHIGREEN << " [MG] User " << clients[index].getId() << 
            " (" << getNameTypeUser(clients[index].getTypeUser()) << " : " << 
            clients[index].getCurrentCredit() << " credits) is being attended" << BHIWHITE << std::endl;
    } else {
        std::cout << BHIGREEN << " [MG] User " << clients[index].getId() << 
            " (" << getNameTypeUser(clients[index].getTypeUser()) << 
            ") is being attended" << BHIWHITE << std::endl;
    }
}

/* Create search requests for users */
void startSearchRequests(std::vector<User> &clients) {

    std::mutex sem;
    std::vector<std::thread> threads;
    std::mutex semSearchRequestQueue;
    int numServedUsers = 0;

    while(allClientServed(clients)){
        
        std::unique_lock<std::mutex> ul(semSearchRequestQueue);
        searchRequestCV.wait(ul, [] {return (searchRequestQueue.size() < CONCURRENT_REQUESTS);});
        ul.unlock();

        int clientPreference = generateRandomNumber(10);
        int index = searchForClientType(clientPreference, clients);

        if (index != -1) { /* Check if there are clients to be served */

            printUserServedInfo(clients, index);
            clients[index].setServed(true); /* Set that client is being served */

            SearchRequest sq(clients[index].getId(), clients[index].getRequestedWord());
            searchRequestQueueMutex.lock(); 
            searchRequestQueue.push(sq);
            std::cout << BHIGREEN << " [MG] Users currently being attended: " << 
                    searchRequestQueue.size() << " (max. " <<  CONCURRENT_REQUESTS << ")" 
                    << BHIWHITE << std::endl;
            searchRequestQueueMutex.unlock(); 
                
            threads.push_back(std::thread(Browser(&sem, &clients[index]))); /* Launch the main browser for each user */

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                
            std::cout << BHIGREEN << " [MG] Remaining users: " << clients.size() - ++numServedUsers << BHIWHITE << std::endl;
        } 
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

/* Delete and create results directory */
void cleanResults() {
    system("rm -r results/");
    system("mkdir results");
}

/* Main function */
int main(int argc, char *argv[]) {
    
    std::vector<User> clients;

    /* Install signal handler */
    installSignalHandler();
    /* Launch payment service */
    PaymentGateway pg;
    std::thread pgThread(std::ref(pg));

    /* Main funcionality */
    cleanResults();
    createUsers(clients);
    startSearchRequests(clients);
 
    /* Notify to payment gateway that program ends */
    endRequest = true;
    paymentGatewayCV.notify_one();
    pgThread.detach();

    return 0;
}   
#endif