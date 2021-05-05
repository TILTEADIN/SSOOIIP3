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
#include <unistd.h>

#include "../include/colors.h"
#include "../src/User.cpp"

#define MAXIMUM_CREDIT 15
#define NUM_CLIENTS 3
#define MAX_LEN 256
#define N_SEARCH_MAX 2
#define MATERIAL_PATH "material2/"

std::queue<SearchRequest> searchRequestQueue;
std::condition_variable searchRequestCV;
std::condition_variable paymentGatewayCV;
std::mutex rechargeCreditRequestMutex, paymentGatewayMutex, searchRequestMutex, searchRequestQueueMutex;
std::queue<User*> rechargeCreditRequestQueue;

int generateRandomNumber(int max){
  usleep(3000);
  srand((unsigned) time(0));
  return ((rand() % max) + 1);
}
#endif