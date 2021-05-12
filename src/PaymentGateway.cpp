/******************************************************************
 * Project          : Práctica 3 de Sistemas Operativos II
 * Program name     : PaymentGateway.cpp
 * Authors          : Alberto Vázquez y Eduardo Eiroa
 * Date created     : 12/05/2021
 * Purpose          : Payment service for top up requests 
 *                    performed by users
 ******************************************************************/

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
            
            //User *user = rechargeCreditRequestQueue.front();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            TopUpRequest request = std::move(rechargeCreditRequestQueue.front());

            std::cout << BHICYAN << " [PG] User " << request.user->getId() << 
                    " requests a credit recharge" << BHIWHITE << std::endl;

            int credit = generateRandomNumber(MAXIMUM_CREDIT);
            
            request.user->setCurrentCredit(credit);
            request.user->setTotalCredit(request.user->getTotalCredit()+credit);

            std::cout << BHICYAN << " [PG] User "<< request.user->getId() << 
                    " account has been recharge with " << request.user->getCurrentCredit() << " credits" << BHIWHITE << std::endl;
            
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            //rechargeCreditRequestMutex.unlock();
            request.clientRequestPromise.set_value(credit);
            rechargeCreditRequestMutex.lock();
            rechargeCreditRequestQueue.pop();
            rechargeCreditRequestMutex.unlock();
        }
    }
};
#endif