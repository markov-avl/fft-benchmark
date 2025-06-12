#include "ImpulseInputGenerator.h"


void ImpulseInputGenerator::fill(ft_complex *in, const size_t size) {
    for (size_t i = 0; i < size; ++i) {
        in[i][0] = size / 2 == i ? 1.0 : 0.0;
        in[i][1] = 0.0;
    }
}
