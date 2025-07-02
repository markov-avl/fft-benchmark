#include "CooleyTukey_R_R8.h"

#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n, const ft_complex *in, ft_complex *out, const size_t T = 1) {
    if (n == 1) {
        FT_COPY(in[0], out[0]);
        return;
    }

    const size_t eighth = n / 8;
    auto *group_0_in = new ft_complex[eighth];
    auto *group_1_in = new ft_complex[eighth];
    auto *group_2_in = new ft_complex[eighth];
    auto *group_3_in = new ft_complex[eighth];
    auto *group_4_in = new ft_complex[eighth];
    auto *group_5_in = new ft_complex[eighth];
    auto *group_6_in = new ft_complex[eighth];
    auto *group_7_in = new ft_complex[eighth];
    auto *group_0_out = new ft_complex[eighth];
    auto *group_1_out = new ft_complex[eighth];
    auto *group_2_out = new ft_complex[eighth];
    auto *group_3_out = new ft_complex[eighth];
    auto *group_4_out = new ft_complex[eighth];
    auto *group_5_out = new ft_complex[eighth];
    auto *group_6_out = new ft_complex[eighth];
    auto *group_7_out = new ft_complex[eighth];

    for (size_t i = 0; i < eighth; ++i) {
        FT_COPY(in[8 * i], group_0_in[i]);
        FT_COPY(in[8 * i + 1], group_1_in[i]);
        FT_COPY(in[8 * i + 2], group_2_in[i]);
        FT_COPY(in[8 * i + 3], group_3_in[i]);
        FT_COPY(in[8 * i + 4], group_4_in[i]);
        FT_COPY(in[8 * i + 5], group_5_in[i]);
        FT_COPY(in[8 * i + 6], group_6_in[i]);
        FT_COPY(in[8 * i + 7], group_7_in[i]);
    }

    if (T > 7) {
        std::thread t1(fft, eighth, group_0_in, group_0_out, T / 8);
        std::thread t2(fft, eighth, group_1_in, group_1_out, T / 8);
        std::thread t3(fft, eighth, group_2_in, group_2_out, T / 8);
        std::thread t4(fft, eighth, group_3_in, group_3_out, T / 8);
        std::thread t5(fft, eighth, group_4_in, group_4_out, T / 8);
        std::thread t6(fft, eighth, group_5_in, group_5_out, T / 8);
        std::thread t7(fft, eighth, group_6_in, group_6_out, T / 8);
        fft(eighth, group_7_in, group_7_out, T / 8);

        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        t7.join();
    } else if (T > 3) {
        std::thread t1([&] {
            fft(eighth, group_0_in, group_0_out);
            fft(eighth, group_1_in, group_1_out);
        });
        std::thread t2([&] {
            fft(eighth, group_2_in, group_2_out);
            fft(eighth, group_3_in, group_3_out);
        });
        std::thread t3([&] {
            fft(eighth, group_4_in, group_4_out);
            fft(eighth, group_5_in, group_5_out);
        });
        fft(eighth, group_6_in, group_6_out);
        fft(eighth, group_7_in, group_7_out);

        t1.join();
        t2.join();
        t3.join();
    } else if (T > 1) {
        std::thread t([&] {
            fft(eighth, group_0_in, group_0_out);
            fft(eighth, group_1_in, group_1_out);
            fft(eighth, group_2_in, group_2_out);
            fft(eighth, group_3_in, group_3_out);
        });
        fft(eighth, group_4_in, group_4_out);
        fft(eighth, group_5_in, group_5_out);
        fft(eighth, group_6_in, group_6_out);
        fft(eighth, group_7_in, group_7_out);

        t.join();
    } else {
        fft(eighth, group_0_in, group_0_out);
        fft(eighth, group_1_in, group_1_out);
        fft(eighth, group_2_in, group_2_out);
        fft(eighth, group_3_in, group_3_out);
        fft(eighth, group_4_in, group_4_out);
        fft(eighth, group_5_in, group_5_out);
        fft(eighth, group_6_in, group_6_out);
        fft(eighth, group_7_in, group_7_out);
    }

    for (size_t k = 0; k < eighth; ++k) {
    }

    delete[] group_0_in;
    delete[] group_1_in;
    delete[] group_2_in;
    delete[] group_3_in;
    delete[] group_4_in;
    delete[] group_5_in;
    delete[] group_6_in;
    delete[] group_7_in;
    delete[] group_0_out;
    delete[] group_1_out;
    delete[] group_2_out;
    delete[] group_3_out;
    delete[] group_4_out;
    delete[] group_5_out;
    delete[] group_6_out;
    delete[] group_7_out;
}


void CooleyTukey_R_R8::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, in, out, get_max_threads());
}
