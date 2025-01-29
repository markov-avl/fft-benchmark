#ifndef UTILS_H
#define UTILS_H
#include "types.h"

void ft_copy(const ft_complex &from, ft_complex &to);

void ft_mul(const ft_complex &a, const ft_complex &b, ft_complex &result);

void ft_mul(ft_complex &a, const ft_complex &b);

void ft_add(const ft_complex &a, const ft_complex &b, ft_complex &result);

void ft_add(ft_complex &a, const ft_complex &b);

void ft_sub(const ft_complex &a, const ft_complex &b, ft_complex &result);

void ft_sub(ft_complex &a, const ft_complex &b);

void ft_polar(double angle, ft_complex &result);

void bit_shuffle(size_t n, const ft_complex *in, ft_complex *out);

void bit_shuffle_multithreaded(size_t n, const ft_complex *in, ft_complex *out);

void fft_of_2(ft_complex *data);

void fft_of_4(ft_complex *data);

void fft_of_8(ft_complex *data);

void transform(size_t half, ft_complex *data);

void transform(size_t half, ft_complex *data, size_t from, size_t to);

#endif
