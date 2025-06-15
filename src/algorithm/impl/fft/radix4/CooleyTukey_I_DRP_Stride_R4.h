#pragma once

#include "Radix4FFT.h"


class CooleyTukey_I_DRP_Stride_R4 final : public Radix4FFT {
public:
    static constexpr auto NAME = "CT-I+DRP+S-4";

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
