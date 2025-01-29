#ifndef FFTWESTIMATE_H
#define FFTWESTIMATE_H
#include <fftw3.h>
#include "algorithm.h"


class FFTWEstimate final : public IFourierTransformAlgorithm {
    fftw_plan plan{};

public:
    FFTWEstimate(): IFourierTransformAlgorithm("FFTW Estimate") {
    }

protected:
    void initialize(size_t n, ft_complex *in, ft_complex *out) override;

    void finalize(size_t n, ft_complex *in, ft_complex *out) override;

    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};


#endif
