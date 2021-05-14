/******************************************************************
 * Project          : Práctica 3 de Sistemas Operativos II
 * Program name     : TopUpRequest.cpp
 * Authors          : Alberto Vázquez y Eduardo Eiroa
 * Date created     : 12/05/2021
 * Purpose          : Class that represents a top up request
 *                    performed by users/clients
 ******************************************************************/


#include <future>
#include "../src/User.cpp"

class TopUpRequest {

public:
    //We are using Promise & Future as a one-use bullet for alterating the value of the User's credit.
    std::promise<int> clientRequestPromise;
    std::future<int> clientRequestFuture;
    User *user;

    TopUpRequest(User *user);
};

TopUpRequest::TopUpRequest(User *user) {
    clientRequestFuture=clientRequestPromise.get_future();
    this->user = user;
}