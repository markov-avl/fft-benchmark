#ifndef RECURSIVESTEPPEDRADIX2_H
#define RECURSIVESTEPPEDRADIX2_H
#include "Radix2FFT.h"


class RecursiveSteppedRadix2FFT final : public Radix2FFT {
public:
    RecursiveSteppedRadix2FFT(): Radix2FFT("FFT-RS-2") {
    }

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};


#endif
