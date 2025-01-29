#ifndef RECURSIVETHREADEDRADIX2FFT_H
#define RECURSIVETHREADEDRADIX2FFT_H
#include "Radix2FFT.h"


class RecursiveThreadedRadix2FFT final : public Radix2FFT {
public:
    RecursiveThreadedRadix2FFT(): Radix2FFT("FFT-R+T-2") {
    }

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};


#endif
