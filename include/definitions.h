#include <cstdlib>
#include <ctime>
#include <condition_variable>
#include <queue>

#define MAXIMUM_CREDIT 15
#define NUM_CLIENTS 3

std::condition_variable cv;
std::mutex rechargeCreditRequestMutex, paymentGatewayMutex;
std::queue<User*> rechargeCreditRequestQueue;

int generateRandomNumber(int max){
  srand((unsigned) time(0));
  return ((rand() % max) + 1);
}