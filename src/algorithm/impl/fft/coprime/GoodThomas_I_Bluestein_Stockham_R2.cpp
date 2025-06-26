#include "GoodThomas_I_Bluestein_Stockham_R2.h"

#include <barrier>
#include <cmath>
#include <numeric>
#include <vector>

#include "algorithm.h"
#include "multiprocessing.h"


static size_t find_optimal_coprime(const size_t n) {
    for (auto n1 = static_cast<size_t>(std::sqrt(n)); n1 > 1; --n1) {
        if (n % n1 == 0 && std::gcd(n1, n / n1) == 1) {
            return n1;
        }
    }

    return 1;
}

static void stockham_forward(const size_t n, ft_complex *sequence) {
    auto *work_area = new ft_complex[n];
    ft_complex *x = sequence;
    ft_complex *y = work_area;

    for (size_t half = n >> 1, s = 1; half > 1; half >>= 1, s <<= 1) {
        const double theta = std::numbers::pi / static_cast<double>(half);

        for (size_t p = 0; p < half; ++p) {
            const double angle = static_cast<double>(p) * theta;
            const ft_complex w = {std::cos(angle), -std::sin(angle)};

            for (size_t q = 0; q < s; ++q) {
                ft_complex a, b;
                FT_COPY(x[q + s * p], a);
                FT_COPY(x[q + s * (p + half)], b);
                FT_ADD(a, b, y[q + s * (2 * p)]);
                FT_SUB(a, b, y[q + s * (2 * p + 1)]);
                FT_RMUL(y[q + s * (2 * p + 1)], w);
            }
        }

        std::swap(x, y);
    }

    const size_t half = n >> 1;
    for (size_t q = 0; q < half; ++q) {
        ft_complex a, b;
        FT_COPY(x[q], a);
        FT_COPY(x[q + half], b);
        FT_ADD(a, b, sequence[q]);
        FT_SUB(a, b, sequence[q + half]);
    }

    delete[] work_area;
}

static void stockham_inverse(const size_t n, ft_complex *sequence) {
    for (size_t i = 0; i < n; ++i) {
        FT_RCONJ(sequence[i]);
    }

    stockham_forward(n, sequence);

    for (size_t i = 0; i < n; ++i) {
        sequence[i][0] = sequence[i][0] / static_cast<double>(n);
        sequence[i][1] = -sequence[i][1] / static_cast<double>(n);
    }
}

static void bluestein_stockham_forward(const size_t n, ft_complex *sequence) {
    size_t l;
    const size_t k = 2 * n - 1;
    for (l = 1; l < k; l <<= 1) {
    }

    auto *u = new ft_complex[l];
    auto *v = new ft_complex[l];
    auto *chirp = new ft_complex[n];

    for (size_t i = 0; i < n; ++i) {
        FT_POLAR(-std::numbers::pi * i * (static_cast<double>(i) / n), chirp[i]);
        FT_MUL(sequence[i], chirp[i], u[i]);
        FT_CONJ(v[i], chirp[i]);
    }

    for (size_t i = n; i < l; ++i) {
        FT_ZERO(u[i]);
        FT_ZERO(v[i]);
    }
    for (size_t i = 1; i < n; ++i) {
        FT_COPY(v[i], v[l - i]);
    }

    stockham_forward(l, u);
    stockham_forward(l, v);

    for (size_t i = 0; i < l; ++i) {
        FT_RMUL(u[i], v[i]);
    }

    stockham_inverse(l, u);

    for (size_t i = 0; i < n; ++i) {
        FT_MUL(u[i], chirp[i], sequence[i]);
    }

    delete[] u;
    delete[] v;
    delete[] chirp;
}

void GoodThomas_I_Bluestein_Stockham_R2::check_preconditions(const size_t n, ft_complex *in, ft_complex *out) {
    n1 = find_optimal_coprime(n);
    n2 = n / n1;
    if (n1 == 1) {
        throw std::invalid_argument("n should be decomposed as coprime");
    }
}

void GoodThomas_I_Bluestein_Stockham_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    const size_t thread_count = get_max_threads();
    std::vector<std::thread> threads;
    std::barrier barrier(static_cast<long>(thread_count));

    auto *x = new ft_complex[n];

    for (size_t k1 = 0; k1 < n1; ++k1) {
        for (size_t k2 = 0; k2 < n2; ++k2) {
            FT_COPY(in[(k1 * n2 + k2 * n1) % n], x[k1 * n2 + k2]);
        }
    }

    auto task = [&](const size_t t) {
        for (size_t k1 = t; k1 < n1; k1 += thread_count) {
            bluestein_stockham_forward(n2, x + k1 * n2);
        }

        barrier.arrive_and_wait();

        auto *temp = new ft_complex[n1];

        for (size_t k2 = t; k2 < n2; k2 += thread_count) {
            for (size_t k1 = 0; k1 < n1; ++k1) {
                FT_COPY(x[k1 * n2 + k2], temp[k1]);
            }
            bluestein_stockham_forward(n1, temp);
            for (size_t k1 = 0; k1 < n1; ++k1) {
                FT_COPY(temp[k1], x[k1 * n2 + k2]);
            }
        }

        delete[] temp;
    };

    for (size_t t = 1; t < thread_count; ++t) {
        threads.emplace_back(task, t);
    }
    task(0);
    for (auto &t: threads) {
        t.join();
    }

    for (size_t k = 0; k < n; ++k) {
        FT_COPY(x[k % n1 * n2 + k % n2], out[k]);
    }

    delete[] x;
}
