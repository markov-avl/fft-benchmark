#ifndef ITERATIVERADIX2FFT_H
#define ITERATIVERADIX2FFT_H
#include "Radix2FFT.h"


class IterativeRadix2FFT final : public Radix2FFT {
public:
    IterativeRadix2FFT(): Radix2FFT("FFT-I-2") {
    }

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};


#endif
