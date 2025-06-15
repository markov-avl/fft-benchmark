#pragma once

#include <cmath>
#include <cstring>

#include "types.h"


void ft_copy(const ft_complex &from, ft_complex &to);

void ft_mul(const ft_complex &a, const ft_complex &b, ft_complex &result);

void ft_mul(ft_complex &a, const ft_complex &b);

void ft_add(const ft_complex &a, const ft_complex &b, ft_complex &result);

void ft_add(ft_complex &a, const ft_complex &b);

void ft_sub(const ft_complex &a, const ft_complex &b, ft_complex &result);

void ft_sub(ft_complex &a, const ft_complex &b);

void ft_polar(double angle, ft_complex &result);

#define FT_COPY(from, to) \
memcpy(&to, &from, sizeof(ft_complex))

#define FT_MUL(a, b, result) \
result[0] = a[0] * b[0] - a[1] * b[1]; \
result[1] = a[0] * b[1] + a[1] * b[0]

#define FT_RMUL(a, b) \
const double t = a[0]; \
a[0] = t * b[0] - a[1] * b[1]; \
a[1] = t * b[1] + a[1] * b[0]

#define FT_ADD(a, b, result) \
result[0] = a[0] + b[0]; \
result[1] = a[1] + b[1]

#define FT_RADD(a, b) \
a[0] += b[0]; \
a[1] += b[1]

#define FT_RSUB(a, b, result) \
result[0] = a[0] - b[0]; \
result[1] = a[1] - b[1]

#define FT_SUB(a, b, result) \
result[0] = a[0] - b[0]; \
result[1] = a[1] - b[1]

#define FT_POLAR(angle, result) \
(result)[0] = std::cos(angle); \
(result)[1] = std::sin(angle)
