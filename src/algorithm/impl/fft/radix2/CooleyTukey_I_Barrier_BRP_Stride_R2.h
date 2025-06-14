#pragma once

#include "Radix2FFT.h"


class CooleyTukey_I_Barrier_BRP_Stride_R2 final : public Radix2FFT {
public:
    static constexpr auto NAME = "CT-I+B+BRP+S-2";

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
