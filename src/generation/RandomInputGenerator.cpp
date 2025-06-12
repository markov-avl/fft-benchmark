#include "RandomInputGenerator.h"
#include <random>


void RandomInputGenerator::fill(ft_complex *in, const size_t size) {
    thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> dist{};

    for (size_t i = 0; i < size; ++i) {
        in[i][0] = dist(gen);
        in[i][1] = dist(gen);
    }
}
