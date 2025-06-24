#include "utils.h"


std::pair<size_t, size_t> thread_range(const size_t n, const size_t t, const size_t thread_count) {
    const size_t q = n / thread_count;
    const size_t r = n % thread_count;
    const size_t start = t * q + (t < r ? t : r);
    const size_t size = q + (t < r);

    return {start, start + size};
}