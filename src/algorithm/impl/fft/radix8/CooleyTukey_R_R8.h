#pragma once

#include "Radix8FFT.h"


class CooleyTukey_R_R8 final : public Radix8FFT {
public:
    static constexpr auto NAME = "CT-R-8";

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
