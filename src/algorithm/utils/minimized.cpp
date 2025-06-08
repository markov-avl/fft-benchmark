#include "minimized.h"

#include <cmath>
#include <emmintrin.h>
#include <numbers>

#include "operation.h"


static constexpr double c4 = std::cos(std::numbers::pi / 4.0);


void fft_of_2_sse(ft_complex *data) {
    const __m128d t1 = _mm_loadu_pd(&data[0][0]);
    const __m128d t2 = _mm_loadu_pd(&data[1][0]);
    const __m128d sum = _mm_add_pd(t1, t2);
    _mm_storeu_pd(&data[0][0], sum);
    const __m128d dif = _mm_sub_pd(t1, t2);
    _mm_storeu_pd(&data[1][0], dif);
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
