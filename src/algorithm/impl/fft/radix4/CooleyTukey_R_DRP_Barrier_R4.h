#pragma once

#include "Radix4FFT.h"


class CooleyTukey_R_DRP_Barrier_R4 final : public Radix4FFT {
public:
    static constexpr auto NAME = "CT-R+B+DRP-4";

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
