#include <iostream>
#include <condition_variable>
#include <queue>
#include <cstdlib>
#include <ctime>

#include "../include/User.h"

#define MAXIMUM_CREDIT 15

std::condition_variable cv; 
std::mutex sem;
std::queue<User> q;

int generateCredit(){
  srand((unsigned) time(0));
  return ((rand() % MAXIMUM_CREDIT) + 1);
}

void waitRequest() {
    std::unique_lock<std::mutex> ul(sem);
    cv.wait(ul, []{return q.empty();});
    User user = q.front();
    q.pop();

    std::cout << "El cliente " << user.getId() << " solicita una recarga de saldo." << std::endl;
    user.setCurrentCredit(generateCredit());
    std::cout << "La cuenta del cliente "<< user.getId() << " ha sido recargada con " << user.getCurrentCredit() << " créditos." << std::endl;
}

int main() {
    waitRequest();
    //std::cout << generateCredit << std::endl;
    return 0;
}

