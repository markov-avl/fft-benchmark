#include <iostream>
#include <random>
#include <fftw3.h>
#include <complex>
#include <memory>
#include "algorithm.h"
#include "output.h"


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

    bool correct = true;
    for (size_t i = 0; i < n; ++i) {
        std::complex fftw_result(out[i][0], out[i][1]);
        std::complex custom_result(custom_output[i][0], custom_output[i][1]);
        if (std::abs(custom_result - fftw_result) > 1e-6) {
            correct = false;
            std::cout << "Test '" << test_name << "' failed at index " << i << ": "
                    << custom_result << " != " << fftw_result << std::endl;
        }
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
