#include "CooleyTukey_R_R4.h"

#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n, const ft_complex *in, ft_complex *out, const size_t thread_count = 1) {
    if (n == 1) {
        FT_COPY(in[0], out[0]);
        return;
    }

    const size_t quarter = n / 4;
    auto* group_0_in = new ft_complex[quarter];
    auto* group_1_in = new ft_complex[quarter];
    auto* group_2_in = new ft_complex[quarter];
    auto* group_3_in = new ft_complex[quarter];
    auto* group_0_out = new ft_complex[quarter];
    auto* group_1_out = new ft_complex[quarter];
    auto* group_2_out = new ft_complex[quarter];
    auto* group_3_out = new ft_complex[quarter];

    for (size_t i = 0; i < quarter; ++i) {
        FT_COPY(in[4 * i], group_0_in[i]);
        FT_COPY(in[4 * i + 1], group_1_in[i]);
        FT_COPY(in[4 * i + 2], group_2_in[i]);
        FT_COPY(in[4 * i + 3], group_3_in[i]);
    }

    if (thread_count > 3) {
        std::thread t1(fft, quarter, group_1_in, group_1_out, thread_count / 4);
        std::thread t2(fft, quarter, group_2_in, group_2_out, thread_count / 4);
        std::thread t3(fft, quarter, group_3_in, group_3_out, thread_count / 4);
        fft(quarter, group_0_in, group_0_out, thread_count / 4);

        t1.join();
        t2.join();
        t3.join();
    } else {
        fft(quarter, group_0_in, group_0_out, thread_count / 4);
        fft(quarter, group_1_in, group_1_out, thread_count / 4);
        fft(quarter, group_2_in, group_2_out, thread_count / 4);
        fft(quarter, group_3_in, group_3_out, thread_count / 4);
    }

    for (size_t k = 0; k < quarter; ++k) {
        ft_complex w1, w2, w3, m, m1w, m2w, m3w;
        FT_POLAR(-0.5 * std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), w1);
        FT_POLAR(-std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), w2);
        FT_POLAR(-1.5 * std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), w3);

        FT_COPY(group_0_out[k], m);
        FT_MUL(group_1_out[k], w1, m1w);
        FT_MUL(group_2_out[k], w2, m2w);
        FT_MUL(group_3_out[k], w3, m3w);

        out[k][0] = m[0] + m1w[0] + m2w[0] + m3w[0];
        out[k][1] = m[1] + m1w[1] + m2w[1] + m3w[1];
        out[k + quarter][0] = m[0] + m1w[1] - m2w[0] - m3w[1];
        out[k + quarter][1] = m[1] - m1w[0] - m2w[1] + m3w[0];
        out[k + 2 * quarter][0] = m[0] - m1w[0] + m2w[0] - m3w[0];
        out[k + 2 * quarter][1] = m[1] - m1w[1] + m2w[1] - m3w[1];
        out[k + 3 * quarter][0] = m[0] - m1w[1] - m2w[0] + m3w[1];
        out[k + 3 * quarter][1] = m[1] + m1w[0] - m2w[1] - m3w[0];
    }

    delete[] group_0_in;
    delete[] group_1_in;
    delete[] group_2_in;
    delete[] group_3_in;
    delete[] group_0_out;
    delete[] group_1_out;
    delete[] group_2_out;
    delete[] group_3_out;
}


void CooleyTukey_R_R4::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, in, out, get_max_threads());
}
