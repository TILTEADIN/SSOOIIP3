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
#include "../src/SemCounter.cpp"

#define NUM_CLIENTS 10
#define CONCURRENT_REQUESTS 2
#define MAX_LEN 256
#define MATERIAL_PATH "material2/"

SemCounter semConcurrentBrowser(CONCURRENT_REQUESTS);
//std::priority_queue<int> requestQueue;
//std::queue<SearchRequest> searchQueue;
std::condition_variable paymentGatewayCV, searchRequestCV;;
std::mutex rechargeCreditRequestMutex, paymentGatewayMutex, searchRequestMutex, searchRequestQueueMutex;
std::queue<User*> rechargeCreditRequestQueue;
std::queue<SearchRequest> searchRequestQueue;

bool endRequest = false;

/* Generate random number between 1 and max */
int generateRandomNumber(int max){
    usleep(1000);
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec); /* using nano-seconds instead of seconds */
    return ((rand() % max) + 1);
}
#endif