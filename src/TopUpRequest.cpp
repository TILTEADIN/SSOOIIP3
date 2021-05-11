#include <future>
#include "../src/User.cpp"

class TopUpRequest{

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