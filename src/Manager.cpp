#ifndef MANAGER
#define MANAGER

#include <csignal>

#include "../src/PaymentGateway.cpp"
#include "../src/Browser.cpp"

std::vector<std::string> diccionary = {"prueba","cuadro","presidente","vendedores",
                                    "titulo","precio","castillo","brujula","beneficios",
                                    "internet","equipo","compañeros","confiabilidad",
                                    "brillante","ojos","comprension","historia"};

//ESTE METODO TIENE QUE ESTAR EN BROWSER o en definitions
/* Request to payment service to recharge credit of a given user */
/*
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
} */   

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

/* Returns a client*/
int searchForClientType(int clientPreference, std::vector<User> clients) {
    int index = -1; /* Returns -1 if there is no user without being attended  */

    for (int i = 0; i < clients.size(); i++) {
        if (!clients[i].getServed()) {
            if (clientPreference > 8){
                if(clients[i].getTypeUser() == 1) {
                    index = i;
                    break;
                }
            } else {
                if(clients[i].getTypeUser() == 2 || clients[i].getTypeUser() == 3){
                    index = i;
                    break;
                }
            } 
        }
    }

    return index;
}
/*
void deleteServedClients(std::vector<User> clients,std::vector<int> clientsToBeErased) { 
            std::cout<<"EL tamaño es :"<<clientsToBeErased.size()<<std::endl;
    while(!clientsToBeErased.empty()){
        clients.erase(clients.begin()+clientsToBeErased.back());
        clientsToBeErased.pop_back();
    }
}*/

/* Main function */
int main(int argc, char *argv[]) {
    
    /* Install signal handler */
    installSignalHandler();

    /* Launch payment service */
    PaymentGateway pg;
    std::thread pgThread(std::ref(pg));

    std::mutex sem;
    std::vector<User> clients;
    std::vector<std::thread> threads;

    /* Create the user's vector */
    for (int i = 1; i <= NUM_CLIENTS; i++) {
        /* Number three is passed as argument for generateRandomNumber so one of the three possible types of user is generated */
        clients.push_back(User(i, generateRandomNumber(3), selectRandomWord()));
    }

    while(!clients.empty()){
        
        for (int i = 0; i < CONCURRENT_REQUESTS; i++){
            int clientPreference = generateRandomNumber(10);
            
            int index = searchForClientType(clientPreference, clients);

            if (index != -1) { /* Check if there are clients to be served */
                std::cout << BHIGREEN << " [MG] El usuario " << clients[index].getId() << 
                        " (" << clients[index].getTypeUser() << ") esta siendo atentido" << BHIWHITE << std::endl;
                clients[index].setServed(true); /* Set client is being served */
                threads.push_back(std::thread(Browser(&sem, &clients[index])));

                std::this_thread::sleep_for(std::chrono::milliseconds(1000));

                //std::cout << index << " " << clients[index].getId() << std::endl;
                clients.erase(clients.begin()+index); /* Delete the served user from clients vector */
                std::cout << BHIGREEN << " [MG] Usuarios restantes: " << clients.size() << BHIWHITE << std::endl;
                semConcurrentBrowser.wait();
                std::cout << semConcurrentBrowser.getValue() << std::endl;
            }
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