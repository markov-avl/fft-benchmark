#pragma once

#include "Radix2FFT.h"


class CooleyTukey_R_InPlace_R2 final : public Radix2FFT {
public:
    static constexpr auto NAME = "CT-R+IP-2";

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
