#pragma once

#include "multiprocessing.h"
#include "Radix4FFT.h"


class Stockham_I_TP_R4 final : public Radix4FFT {
    ThreadPool* thread_pool = nullptr;

public:
    static constexpr auto NAME = "S-I+TP-4";

protected:
    void initialize(size_t n, ft_complex *in, ft_complex *out) override;

    void finalize(size_t n, ft_complex *in, ft_complex *out) override;

    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
