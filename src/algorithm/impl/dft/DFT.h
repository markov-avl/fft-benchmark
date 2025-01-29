#ifndef DFT_H
#define DFT_H
#include "algorithm.h"


class DFT final : public IFourierTransformAlgorithm {
public:
    DFT(): IFourierTransformAlgorithm("DFT") {
    }

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};

#endif
