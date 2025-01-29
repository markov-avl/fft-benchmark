#include <bit>
#include <cmath>
#include <cstring>
#include <thread>
#include <vector>
#include "utils.h"

unsigned char reverse_bits[] = {
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

constexpr double c4 = std::cos(std::numbers::pi_v<double> / 4.0);

uint32_t bit_reverse32(const uint32_t i) {
    return reverse_bits[i & 0xff] << 56 |
           reverse_bits[i >> 8 & 0xff] << 48 |
           reverse_bits[i >> 16 & 0xff] << 40 |
           reverse_bits[i >> 24 & 0xff] << 32 |
           reverse_bits[i >> 32 & 0xff] << 24 |
           reverse_bits[i >> 40 & 0xff] << 16 |
           reverse_bits[i >> 48 & 0xff] << 8 |
           reverse_bits[i >> 56 & 0xff];
}

uint64_t bit_reverse64(const uint64_t i) {
    return static_cast<uint64_t>(reverse_bits[i & 0xff]) << 56 |
           static_cast<uint64_t>(reverse_bits[i >> 8 & 0xff]) << 48 |
           static_cast<uint64_t>(reverse_bits[i >> 16 & 0xff]) << 40 |
           static_cast<uint64_t>(reverse_bits[i >> 24 & 0xff]) << 32 |
           static_cast<uint64_t>(reverse_bits[i >> 32 & 0xff]) << 24 |
           static_cast<uint64_t>(reverse_bits[i >> 40 & 0xff]) << 16 |
           static_cast<uint64_t>(reverse_bits[i >> 48 & 0xff]) << 8 |
           static_cast<uint64_t>(reverse_bits[i >> 56 & 0xff]);
}

void ft_copy(const ft_complex &from, ft_complex &to) {
    memcpy(&to, &from, sizeof(ft_complex));
}

void ft_mul(const ft_complex &a, const ft_complex &b, ft_complex &result) {
    result[0] = a[0] * b[0] - a[1] * b[1];
    result[1] = a[0] * b[1] + a[1] * b[0];
}

void ft_mul(ft_complex &a, const ft_complex &b) {
    const double t = a[0];
    a[0] = t * b[0] - a[1] * b[1];
    a[1] = t * b[1] + a[1] * b[0];
}

void ft_add(const ft_complex &a, const ft_complex &b, ft_complex &result) {
    result[0] = a[0] + b[0];
    result[1] = a[1] + b[1];
}

void ft_add(ft_complex &a, const ft_complex &b) {
    a[0] += b[0];
    a[1] += b[1];
}

void ft_sub(const ft_complex &a, const ft_complex &b, ft_complex &result) {
    result[1] = a[1] - b[1];
    result[0] = a[0] - b[0];
}

void ft_sub(ft_complex &a, const ft_complex &b) {
    a[0] -= b[0];
    a[1] -= b[1];
}

void ft_polar(const double angle, ft_complex &result) {
    result[0] = std::cos(angle);
    result[1] = std::sin(angle);
}

void bit_shuffle(const size_t n, const ft_complex *in, ft_complex *out) {
    const size_t shift = 64 - std::countr_zero(n);
    for (size_t i = 0; i < n; ++i) {
        ft_copy(in[i], out[bit_reverse64(i) >> shift]);
    }
}

void bit_shuffle_multithreaded(const size_t n, const ft_complex *in, ft_complex *out) {
    const size_t shift = 64 - std::countr_zero(n);

    if (n <= 32768) {
#pragma omp parallel for
        for (size_t i = 0; i < n; ++i) {
            ft_copy(in[i], out[bit_reverse64(i) >> shift]);
        }
        return;
    }

    const size_t thread_count = std::thread::hardware_concurrency();
    const size_t batch_size = std::llround(n / thread_count);
    std::vector<std::thread> threads;

    auto shuffle = [&](const size_t t) {
        const size_t start = t * batch_size;
        const size_t end = std::min(start + batch_size, n);
        for (size_t i = start; i < end; ++i) {
            ft_copy(in[i], out[bit_reverse64(i) >> shift]);
        }
    };

    for (size_t i = 1; i < thread_count; ++i) {
        threads.emplace_back(shuffle, i);
    }

    shuffle(0);

    for (auto &t: threads) {
        t.join();
    }
}

void fft_of_2(ft_complex *data) {
    const ft_complex t = {data[0][0], data[0][1]};
    ft_add(data[0], data[1]);
    ft_sub(t, data[1], data[1]);
}

void fft_of_4(ft_complex *data) {
    fft_of_2(data);
    fft_of_2(data + 2);

    ft_complex t = {data[2][0], data[2][1]};
    ft_sub(data[0], t, data[2]);
    ft_add(data[0], t);
    t[0] = data[3][1], t[1] = -data[3][0];
    ft_sub(data[1], t, data[3]);
    ft_add(data[1], t);
}

void fft_of_8(ft_complex *data) {
    fft_of_4(data);
    fft_of_4(data + 4);

    ft_complex t = {data[4][0], data[4][1]};
    ft_sub(data[0], t, data[4]);
    ft_add(data[0], t);
    t[0] = (data[5][0] + data[5][1]) * c4, t[1] = (data[5][1] - data[5][0]) * c4;
    ft_sub(data[1], t, data[5]);
    ft_add(data[1], t);
    t[0] = data[6][1], t[1] = -data[6][0];
    ft_sub(data[2], t, data[6]);
    ft_add(data[2], t);
    t[0] = (data[7][1] - data[7][0]) * c4, t[1] = -(data[7][0] + data[7][1]) * c4;
    ft_sub(data[3], t, data[7]);
    ft_add(data[3], t);
}

void transform(const size_t half, ft_complex *data) {
    transform(half, data, 0, half);
}

void transform(const size_t half, ft_complex *data, const size_t from, const size_t to) {
    ft_complex t;
    for (size_t k = from; k < to; ++k) {
        ft_polar(-std::numbers::pi_v<double> * static_cast<double>(k) / static_cast<double>(half), t);
        ft_mul(t, data[half + k]);
        ft_sub(data[k], t, data[half + k]);
        ft_add(data[k], t);
    }
}
