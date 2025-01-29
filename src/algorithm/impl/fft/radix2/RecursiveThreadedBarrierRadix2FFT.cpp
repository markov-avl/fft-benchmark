#include <barrier>
#include <thread>
#include "algorithm.h"
#include "RecursiveThreadedBarrierRadix2FFT.h"


static void fft(const size_t n, ft_complex *data, const size_t thread_count) {
    if (n == 8) {
        fft_of_8(data);
        return;
    }
    if (n == 4) {
        fft_of_4(data);
        return;
    }
    if (n == 2) {
        fft_of_2(data);
        return;
    }
    if (n == 1) {
        return;
    }

    const size_t half = n / 2;

    if (thread_count > 1) {
        std::barrier barrier(2);
        std::thread executor([&] {
            fft(half, data, thread_count / 2);
            barrier.arrive_and_wait();
            transform(half, data, 0, half / 2);
        });

        fft(half, data + half, thread_count / 2);
        barrier.arrive_and_wait();
        transform(half, data, half / 2, half);
        executor.join();

        return;
    }

    fft(half, data, 1);
    fft(half, data + half, 1);
    transform(half, data);
}

void RecursiveThreadedBarrierRadix2FFT::forward(const size_t n, ft_complex *in, ft_complex *out) {
    bit_shuffle_multithreaded(n, in, out);
    fft(n, out, std::thread::hardware_concurrency());
}
