#ifndef DEFI
#define DEFI
#include <cstdlib>
#include <ctime>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <algorithm>
#include <functional>
#include <vector>

#include "../include/colors.h"
#include "../src/User.cpp"

#define MAXIMUM_CREDIT 15
#define NUM_CLIENTS 2
#define MAX_LEN 256
#define MATERIAL_PATH "material2/"

std::queue<SearchRequest> searchRequestQueue;
std::condition_variable searchRequestCV;
std::condition_variable paymentGatewayCV;
std::mutex rechargeCreditRequestMutex, paymentGatewayMutex, searchRequestMutex, searchRequestQueueMutex;
std::queue<User*> rechargeCreditRequestQueue;

int generateRandomNumber(int max){
  srand((unsigned) time(0));
  return ((rand() % max) + 1);
}
#endif