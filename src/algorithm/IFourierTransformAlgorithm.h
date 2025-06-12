#pragma once

#include "measurement.h"
#include "types.h"


class IFourierTransformAlgorithm {
public:
    virtual ~IFourierTransformAlgorithm() = default;

    [[nodiscard]] virtual int supported_sequences() const = 0;

    void run(size_t n, ft_complex *in, ft_complex *out);

    void run(size_t n, ft_complex *in, ft_complex *out, IMeasurer *measurer);

protected:
    virtual void check_preconditions(size_t n, ft_complex *in, ft_complex *out);

    virtual void initialize(size_t n, ft_complex *in, ft_complex *out);

    virtual void finalize(size_t n, ft_complex *in, ft_complex *out);

    virtual void forward(size_t n, ft_complex *in, ft_complex *out) = 0;
};
