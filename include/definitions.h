#include <cstdlib>
#include <ctime>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <algorithm>
#include <functional>
#include <vector>

#include "../include/User.h"
#include "../include/colors.h"

#define MAXIMUM_CREDIT 15
#define NUM_CLIENTS 3
#define MAX_LEN 256
#define MATERIAL_PATH "material/"

std::condition_variable cv;
std::mutex rechargeCreditRequestMutex, paymentGatewayMutex;
std::queue<User*> rechargeCreditRequestQueue;

int generateRandomNumber(int max){
  srand((unsigned) time(0));
  return ((rand() % max) + 1);
}