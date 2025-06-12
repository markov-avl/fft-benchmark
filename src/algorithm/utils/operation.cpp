#include "operation.h"

#include <cmath>
#include <cstring>


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
