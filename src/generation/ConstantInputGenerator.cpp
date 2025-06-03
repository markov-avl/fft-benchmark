#include "ConstantInputGenerator.h"


void ConstantInputGenerator::fill(ft_complex *in, const size_t size) {
    for (size_t i = 0; i < size; ++i) {
        in[i][0] = 1.0;
        in[i][1] = 0.0;
    }
}
