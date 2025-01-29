#ifndef FT_ALGORITHM_H
#define FT_ALGORITHM_H
#include <string>
#include "types.h"
#include "measurement.h"


class IFourierTransformAlgorithm {
    std::string name;

public:
    virtual ~IFourierTransformAlgorithm() = default;

    explicit IFourierTransformAlgorithm(std::string name);

    void run(size_t n, ft_complex *in, ft_complex *out);

    void run(size_t n, ft_complex *in, ft_complex *out, IMeasurer *measurer);

    [[nodiscard]] const std::string &id() const;

protected:
    virtual void check_preconditions(size_t n, ft_complex *in, ft_complex *out);

    virtual void initialize(size_t n, ft_complex *in, ft_complex *out);

    virtual void finalize(size_t n, ft_complex *in, ft_complex *out);

    virtual void forward(size_t n, ft_complex *in, ft_complex *out) = 0;
};


#endif
