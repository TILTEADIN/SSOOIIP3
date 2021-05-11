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
#include "../src/SemCounter.cpp"
#include "../src/TopUpRequest.cpp"

#define NUM_CLIENTS 5
#define CONCURRENT_REQUESTS 5
#define MAX_LEN 256
#define MATERIAL_PATH "material/"

SemCounter semConcurrentBrowser(CONCURRENT_REQUESTS);

std::condition_variable paymentGatewayCV, searchRequestCV;;
std::mutex rechargeCreditRequestMutex, paymentGatewayMutex, searchRequestMutex, searchRequestQueueMutex;
std::queue<TopUpRequest> rechargeCreditRequestQueue;
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