#ifndef _PAYMENT_GATEWAY_
#define _PAYMENT_GATEWAY_

#include <iostream>
#include <condition_variable>
#include <queue>
#include <thread>

#include "../include/definitions.h"

class PaymentGateway {
public:

    PaymentGateway(){};

    ~PaymentGateway(){
        std::cout << BHICYAN << " [PG] Payment Gateway is shutting down" << BHIWHITE << std::endl;
    };

    void operator()() {
        waitRequest();
    }

    void waitRequest() {

        std::cout << BHICYAN << " [PG] Waiting for top up requests... " << BHIWHITE << std::endl;
        while (1) {
            std::unique_lock<std::mutex> ul(paymentGatewayMutex);
            cv.wait(ul, [] {return (!rechargeCreditRequestQueue.empty());});

            User *user = rechargeCreditRequestQueue.front();
            rechargeCreditRequestQueue.pop();

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << BHICYAN << " [PG] El cliente " << user->getId() << 
                    " solicita una recarga de saldo." << BHIWHITE << std::endl;

            user->setCurrentCredit(generateRandomNumber(MAXIMUM_CREDIT));

            std::cout << BHICYAN << " [PG] La cuenta del cliente "<< user->getId() << 
                    " ha sido recargada con " << user->getCurrentCredit() << " créditos." << BHIWHITE << std::endl;
            
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            rechargeCreditRequestMutex.unlock();
        }
    }
    



};
#endif