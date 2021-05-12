/******************************************************************
 * Project          : Práctica 3 de Sistemas Operativos II
 * Program name     : TopUpRequest.cpp
 * Authors          : Alberto Vázquez y Eduardo Eiroa
 * Date created     : 12/05/2021
 * Purpose          : Class that represent a recharge request 
 *                    performed by users/clients
 ******************************************************************/


#include <future>
#include "../src/User.cpp"

class TopUpRequest {

public:
    std::promise<int> clientRequestPromise;
    std::future<int> clientRequestFuture;
    User *user;

    TopUpRequest(User *user);
};

TopUpRequest::TopUpRequest(User *user) {
    clientRequestFuture=clientRequestPromise.get_future();
    this->user = user;
}