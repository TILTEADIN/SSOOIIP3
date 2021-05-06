#ifndef MANAGER
#define MANAGER

#include <csignal>

#include "../src/PaymentGateway.cpp"
#include "../src/Browser.cpp"

std::vector<std::string> diccionary = {"prueba","cuadro","presidente","vendedores",
                                    "titulo","precio","castillo","brujula","beneficios",
                                    "internet","equipo","compeñeros","confiabilidad",
                                    "brillante","ojos","comprension","historia"};

//ESTE METODO TIENE QUE ESTAR EN BROWSER o en definitions
/* Request to payment service to recharge credit of a given user */
void requestCreditRecharge(User *user) {
    rechargeCreditRequestMutex.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << BHIGREEN << " [MG] User n: " << user->getId() << 
                " requests a credit top up " << BHIWHITE << std::endl;

    rechargeCreditRequestQueue.push(user);
    paymentGatewayCV.notify_one();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << BHIGREEN << " [MG] User's credit" << user->getId() << " topped up to " 
        << user->getCurrentCredit() << " credits." << BHIWHITE << std::endl;
}    

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

/* Ret */
int searchForClientType(int clientPreference,std::vector<User> clients) {
    int index;
    
    for (int i = 0; i < NUM_CLIENTS; i++) {
        if (clientPreference > 7 ){
            if(clients[i].getTypeUser() == 0)
                index = i;
        } else {
            if(clients[i].getTypeUser() == 1 || clients[i].getTypeUser() == 2)
                index = i;
        }
    }

    return index;
}


void deleteServedClients(std::vector<User> clients,std::vector<int> clientsToBeErased) { 
            std::cout<<"EL tamaño es :"<<clientsToBeErased.size()<<std::endl;
    while(!clientsToBeErased.empty()){
        clients.erase(clients.begin()+clientsToBeErased.back());
        clientsToBeErased.pop_back();
    }
}

/* Main function */
int main(int argc, char *argv[]) {
    /* Install signal handler*/
    installSignalHandler();
    /* Launch payment service */
    PaymentGateway pg;
    std::thread pgThread(std::ref(pg));

    std::vector<User> clients;
    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_CLIENTS; i++) {
        //std::cout << i << std::endl;
        //std::queue<SearchRequest> searchRequestQueue1;
        //searchRequestQueue1.push(SearchRequest(i,selectRandomWord()));
        
        //Number three is passed as argument for generateRandomNumber so one of the three possible types of user is generated
        clients.push_back(User(i, generateRandomNumber(3), selectRandomWord()));
    }
    std::mutex sem;

    while(!clients.empty()){
        std::cout<<"entrando en el bucle"<<std::endl;
        std::vector<int> clientsToBeErased;
        
        for (int i = 0; i < CONCURRENT_BROWSERS; i++){
            int clientPreference = generateRandomNumber(9);
            std::cout<<"Preferencia es: "<<clientPreference<<std::endl;
            if (clientPreference > 7){
                //Enqueue free client
                clientsToBeErased.push_back(searchForClientType(clientPreference,clients));
                threads.push_back(std::thread(Browser(&sem, &clients[clientsToBeErased.back()])));
            } else {
                //Enqueue VIP client
                clientsToBeErased.push_back(searchForClientType(clientPreference,clients));
                threads.push_back(std::thread(Browser(&sem, &clients[clientsToBeErased.back()])));
            }
            semConcurrentBrowser.wait();
            deleteServedClients(clients,clientsToBeErased);
        }   
        
    }

    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    
    /* Notify to payment gateway that program ends */
    endRequest = true;
    paymentGatewayCV.notify_one();
    pgThread.detach();

    return 0;
}   
#endif

/*
-Comprobar el saldo en resultado encontrado.
-Decrementar saldo de cliente no premium
-Recargar saldo a los cliente premium normales(Integrar el servio de pago)
-80% y 20% en la cola de peticiones.
que al comienzo llegen todas las peticiones y dependiendo del tipo de usuario meta a unos a otros??
-Meter los resutlados en ficheros.
*/