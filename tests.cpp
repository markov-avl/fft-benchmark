#include <algorithm>
#include <cmath>
#include <complex>
#include <fftw3.h>
#include <iostream>
#include <memory>
#include <ranges>

#include "algorithm.h"
#include "generation.h"
#include "output.h"


template<class FP>
static FP abs_error_ulps(FP f1, FP f2) {
    if (f1 == f2)
        return 0;
    if (!f1)
        return f2;
    if (!f2)
        return f1;
    return (f2 - f1) / (std::nextafter(f1, f2) - f1);
}

static bool compare_floats(auto f1, auto f2, auto ulp_tolerance) {
    return abs_error_ulps(f1, f2) <= ulp_tolerance;
}

template<class Flt>
static bool compare_complex(const std::complex<Flt> &left, const std::complex<Flt> &right, Flt ulp_tolerance_real,
                            Flt ulp_tolerance_imag) {
    return compare_floats(left.real(), right.real(), ulp_tolerance_real) && compare_floats(
               left.imag(), right.imag(), ulp_tolerance_imag);
}

template<class Flt>
static bool compare_complex(const std::complex<Flt> &left, const std::complex<Flt> &right, Flt ulp_tolerance_real) {
    return compare_complex(left, right, ulp_tolerance_real, ulp_tolerance_real);
}


std::string test_with_fftw(const size_t n, ft_complex *in, IFourierTransformAlgorithm *algorithm) {
    auto *fftw_out = static_cast<ft_complex *>(malloc(sizeof(ft_complex) * n));
    auto *out = static_cast<ft_complex *>(malloc(sizeof(ft_complex) * n));

    const fftw_plan p = fftw_plan_dft_1d(static_cast<int>(n), in, fftw_out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    algorithm->run(n, in, out);

#ifdef TEST_INCORRECT
    auto compare = [](auto out1, auto out2) {
        return std::abs(std::complex{out2[0], out2[1]} - std::complex{out1[0], out1[1]}) < 1E-6;
    };
#else
    const double ulp_tolerance = n * n;
    auto compare = [ulp_tolerance](auto custom, auto out) {
        return compare_complex(custom, out, ulp_tolerance);
    };
#endif
    const long off = std::mismatch(&fftw_out[0], &fftw_out[n], &out[0], compare).first - &fftw_out[0];
    std::string result;

    if (off != n) {
        std::ostringstream oss;
        oss << "Failed at index " << off << ": "
                << std::complex{fftw_out[off][0], fftw_out[off][1]} << " != "
                << std::complex{out[off][0], out[off][1]}
                << ", ERROR: ulp=(" << abs_error_ulps(fftw_out[off][0], out[off][0]) << ", "
                << abs_error_ulps(fftw_out[off][1], out[off][1]) << ")";

        result = oss.str();
    } else {
        result = "OK";
    }

    fftw_destroy_plan(p);
    free(fftw_out);
    free(out);

    return result;
}

std::string run_comparison(IFourierTransformAlgorithm *algorithm, IInputGenerator *generator) {
    size_t n;
    if (algorithm->supported_sequences() == ANY) {
        n = 1000;
    } else if (algorithm->supported_sequences() & PRIME) {
        n = 1009;
    } else if (algorithm->supported_sequences() & COPRIME) {
        n = 1023;
    } else {
        n = 1;
        if (algorithm->supported_sequences() & BASE_OF_2) {
            n *= 1024;
        }
        if (algorithm->supported_sequences() & BASE_OF_3) {
            n *= 2187;
        }
        if (algorithm->supported_sequences() & BASE_OF_4) {
            n *= 1024;
        }
        if (algorithm->supported_sequences() & BASE_OF_5) {
            n *= 3125;
        }
    }

    const auto input = new ft_complex[n];
    generator->fill(input, n);

    const std::string result = test_with_fftw(n, input, algorithm);
    delete[] input;

    return result + " (" + std::to_string(n) + ")";
}

int main() {
    StdOutputCollector output(15, true);

    output.add("Algorithm");
    for (const auto &generator: supported_generators()) {
        output.add(generator);
    }
    output.newline();

    for (const auto &[name, algorithm]: ALGORITHMS) {
        output.add(name);
        for (const auto &generator: GENERATORS | std::views::values) {
            output.add(run_comparison(algorithm.get(), generator.get()));
        }
        output.newline();
    }

    return 0;
}
