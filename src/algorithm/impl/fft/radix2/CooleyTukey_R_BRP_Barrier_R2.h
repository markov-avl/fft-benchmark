#pragma once

#include "Radix2FFT.h"


class CooleyTukey_R_BRP_Barrier_R2 final : public Radix2FFT {
public:
    static constexpr auto NAME = "CT-R+BRP+B-2";

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
