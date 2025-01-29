#ifndef RECURSIVETHREADEDBARRIERRADIX2FFT_H
#define RECURSIVETHREADEDBARRIERRADIX2FFT_H
#include "Radix2FFT.h"


class RecursiveThreadedBarrierRadix2FFT final : public Radix2FFT {
public:
    RecursiveThreadedBarrierRadix2FFT(): Radix2FFT("FFT-RB+T-2") {
    }

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};


#endif
