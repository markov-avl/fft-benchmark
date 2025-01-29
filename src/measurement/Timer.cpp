#include "Timer.h"


void Timer::start() {
    t0 = std::chrono::high_resolution_clock::now();
}

void Timer::stop() {
    t1 = std::chrono::high_resolution_clock::now();
}

double Timer::result() {
    return std::chrono::duration_cast<std::chrono::duration<double> >(t1 - t0).count();
}
