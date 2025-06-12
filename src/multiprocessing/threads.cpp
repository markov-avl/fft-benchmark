#include "threads.h"
#include <omp.h>
#include <thread>


static size_t max_threads = get_system_threads();

void set_max_threads(const size_t n) {
    max_threads = n;
    omp_set_num_threads(static_cast<int>(n));
}

size_t get_max_threads() {
    return max_threads;
}

size_t get_system_threads() {
    return std::thread::hardware_concurrency();
}
