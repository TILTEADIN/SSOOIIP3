#include <cstdlib>
#include <ctime>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <algorithm>
#include <functional>
#include <vector>

#include "../include/User.h"
#include "../include/Browser.h"
#include "../include/Result.h"
#include "../include/SearchRequest.h"
#include "../include/colors.h"
#include "../include/User.h"


#define MAXIMUM_CREDIT 15
#define NUM_CLIENTS 3
#define MAX_LEN 256

std::condition_variable cv;
std::mutex rechargeCreditRequestMutex, paymentGatewayMutex;
std::queue<User*> rechargeCreditRequestQueue;

int generateRandomNumber(int max){
  srand((unsigned) time(0));
  return ((rand() % max) + 1);
}