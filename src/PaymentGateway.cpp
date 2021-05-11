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
            paymentGatewayCV.wait(ul, [] {return (!rechargeCreditRequestQueue.empty() || endRequest);});
            
            User *user = rechargeCreditRequestQueue.front();
            rechargeCreditRequestQueue.pop();

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << BHICYAN << " [PG] User " << user->getId() << 
                    " requests a credit recharge" << BHIWHITE << std::endl;

            int credit = generateRandomNumber(MAXIMUM_CREDIT);
            user->setCurrentCredit(credit);
            user->setTotalCredit(user->getTotalCredit()+credit);

            std::cout << BHICYAN << " [PG] User "<< user->getId() << 
                    " account has been recharge with " << user->getCurrentCredit() << " credits" << BHIWHITE << std::endl;
            
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            rechargeCreditRequestMutex.unlock();
        }
    }
};
#endif