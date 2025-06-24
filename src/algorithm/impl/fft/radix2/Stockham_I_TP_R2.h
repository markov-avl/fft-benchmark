#pragma once

#include "multiprocessing.h"
#include "Radix2FFT.h"


class Stockham_I_TP_R2 final : public Radix2FFT {
    ThreadPool* thread_pool = nullptr;

public:
    static constexpr auto NAME = "S-I+TP-2";

protected:
    void initialize(size_t n, ft_complex *in, ft_complex *out) override;

    void finalize(size_t n, ft_complex *in, ft_complex *out) override;

    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
