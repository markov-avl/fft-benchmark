#ifndef RECURSIVERADIX2FFT_H
#define RECURSIVERADIX2FFT_H
#include "Radix2FFT.h"


class RecursiveRadix2FFT final : public Radix2FFT {
public:
    RecursiveRadix2FFT(): Radix2FFT("FFT-R-2") {
    }

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};


#endif
