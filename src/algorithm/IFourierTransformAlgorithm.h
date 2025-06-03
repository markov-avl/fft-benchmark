#pragma once

#include "measurement.h"
#include "types.h"


class IFourierTransformAlgorithm {
public:
    virtual ~IFourierTransformAlgorithm() = default;

    void run(size_t n, ft_complex *in, ft_complex *out);

    void run(size_t n, ft_complex *in, ft_complex *out, IMeasurer *measurer);

protected:
    virtual void check_preconditions(size_t n, ft_complex *in, ft_complex *out);

    virtual void initialize(size_t n, ft_complex *in, ft_complex *out);

    virtual void finalize(size_t n, ft_complex *in, ft_complex *out);

    virtual void forward(size_t n, ft_complex *in, ft_complex *out) = 0;
};
