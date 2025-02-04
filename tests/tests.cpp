#include <iostream>
#include <random>
#include <fftw3.h>
#include <complex>
#include <memory>
#include <cmath>
#include <bit>
#include <algorithm>
#include "algorithm.h"
#include "output.h"

template <class FP>
static FP abs_error_ulps(FP f1, FP f2) {
	if (f1 == f2)
		return 0;
	if (!f1)
		return f2;
	else if (!f2)
		return f1;
	return (f2 - f1) / (std::nextafter(f1, f2) - f1);
}

static bool compare_floats(auto f1, auto f2, auto ulp_tolerance) {
	return abs_error_ulps(f1, f2) <= ulp_tolerance;
}
template <class Flt>
static bool compare_complex(const std::complex<Flt>& left, const std::complex<Flt>& right, Flt ulp_tolerance_real, Flt ulp_tolerance_imag) {
	return compare_floats(left.real(), right.real(), ulp_tolerance_real) && compare_floats(left.imag(), right.imag(), ulp_tolerance_imag);
}
template <class Flt>
static bool compare_complex(const std::complex<Flt>& left, const std::complex<Flt>& right, Flt ulp_tolerance_real) {
	return compare_complex(left, right, ulp_tolerance_real, ulp_tolerance_real);
}

static bool compare_complex(ft_complex left, ft_complex right, double ulp_tolerance_real, double ulp_tolerance_imag) {
	return compare_floats(left[0], right[0], ulp_tolerance_real) && compare_floats(left[1], right[1], ulp_tolerance_imag);
}
static bool compare_complex(ft_complex left, ft_complex right, double ulp_tolerance_real) {
	return compare_complex(left, right, ulp_tolerance_real, ulp_tolerance_real);
}

bool test_with_fftw(const std::string &test_name,
                    const size_t n,
                    ft_complex *input,
                    IFourierTransformAlgorithm *algorithm) {
    auto *in = static_cast<ft_complex *>(malloc(sizeof(ft_complex) * n));
    auto *out = static_cast<ft_complex *>(malloc(sizeof(ft_complex) * n));
    auto *custom_output = static_cast<ft_complex *>(malloc(sizeof(ft_complex) * n));

    fftw_plan p = fftw_plan_dft_1d(static_cast<int>(n), in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    for (size_t i = 0; i < n; ++i) {
        in[i][0] = input[i][0];
        in[i][1] = input[i][1];
    }

    algorithm->run(n, input, custom_output);
    fftw_execute(p);

    double ulp_tolerance = n*n;
#ifdef TEST_INCORRECT
    auto compare = [](auto custom, auto out) {return std::abs(std::complex{out[0], out[1]} - std::complex{custom[0], custom[1]}) < 1E-6;};
#else
    auto compare = [ulp_tolerance](auto custom, auto out) {return compare_complex(custom, out, ulp_tolerance);};
#endif //TEST_INCORRECT
    auto off = std::mismatch(&out[0], &out[n], &custom_output[0], compare).first - &out[0];
    bool correct = off == n;
    if (!correct) {
    	std::cout << "Test '" << test_name << "' failed at index " << off << ": "
	    << std::complex{out[off][0], out[off][1]} << " != " << std::complex{custom_output[off][0], custom_output[off][1]}
	    << ", ERROR: (" << abs_error_ulps(out[off][0], custom_output[off][0]) << ", " << abs_error_ulps(out[off][1], custom_output[off][1]) << ") ulp;\n";
    }

    fftw_destroy_plan(p);
    free(in);
    free(out);
    free(custom_output);

    return correct;
}

void run_tests(StdOutputCollector &output, IFourierTransformAlgorithm *algorithm) {
    output.add(algorithm->id());

    // Тест 1: Единичный импульс
    {
        constexpr size_t n = 4;
        ft_complex input[n] = {{1, 0}, {0, 0}, {0, 0}, {0, 0}};
        if (test_with_fftw("Impulse", n, input, algorithm)) {
            output.add("OK");
        } else {
            output.add("FAIL");
        }
    }

    // Тест 2: Константный сигнал
    {
        constexpr size_t n = 4;
        ft_complex input[n] = {{1, 0}, {1, 0}, {1, 0}, {1, 0}};
        if (test_with_fftw("Constant Signal", n, input, algorithm)) {
            output.add("OK");
        } else {
            output.add("FAIL");
        }
    }

    // Тест 3: Синусоида
    {
        constexpr size_t n = 1024;
        ft_complex input[n];
        for (size_t k = 0; k < n; ++k) {
            input[k][0] = std::cos(2 * M_PI * static_cast<double>(k) / n);
            input[k][1] = std::sin(2 * M_PI * static_cast<double>(k) / n);
        }
        if (test_with_fftw("Sinusoidal", n, input, algorithm)) {
            output.add("OK");
        } else {
            output.add("FAIL");
        }
    }

    // Тест 4: Случайные комплексные числа
    {
        constexpr size_t n = 1024;
        ft_complex input[n];
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution dis(0.0, 1.0);

        for (auto &i: input) {
            i[0] = dis(gen);
            i[1] = dis(gen);
        }

        if (test_with_fftw("Random Complex", n, input, algorithm)) {
            output.add("OK");
        } else {
            output.add("FAIL");
        }
    }

    output.newline();
}

int main() {
    constexpr size_t num_columns = 5;
    constexpr size_t column_width = 15;
    StdOutputCollector output(num_columns, column_width, true);

    output.add("Algorithm");
    output.add("Impulse");
    output.add("Constant Signal");
    output.add("Sinusoidal");
    output.add("Random Complex");
    output.newline();

    run_tests(output, std::make_unique<DFT>().get());
    run_tests(output, std::make_unique<RecursiveRadix2FFT>().get());
    run_tests(output, std::make_unique<RecursiveSteppedRadix2FFT>().get());
    run_tests(output, std::make_unique<RecursiveThreadedRadix2FFT>().get());
    run_tests(output, std::make_unique<RecursiveThreadedBarrierRadix2FFT>().get());
    run_tests(output, std::make_unique<IterativeRadix2FFT>().get());

    return 0;
}
