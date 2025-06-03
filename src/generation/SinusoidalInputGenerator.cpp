#include "SinusoidalInputGenerator.h"

#include <cmath>
#include <numbers>


void SinusoidalInputGenerator::fill(ft_complex *in, const size_t size) {
    for (size_t i = 0; i < size; ++i) {
        in[i][0] = std::cos(2 * std::numbers::pi_v<double> * static_cast<double>(i) / size);
        in[i][1] = std::sin(2 * std::numbers::pi_v<double> * static_cast<double>(i) / size);
    }
}
