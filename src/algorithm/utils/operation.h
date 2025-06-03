#pragma once

#include "types.h"


void ft_copy(const ft_complex &from, ft_complex &to);

void ft_mul(const ft_complex &a, const ft_complex &b, ft_complex &result);

void ft_mul(ft_complex &a, const ft_complex &b);

void ft_add(const ft_complex &a, const ft_complex &b, ft_complex &result);

void ft_add(ft_complex &a, const ft_complex &b);

void ft_sub(const ft_complex &a, const ft_complex &b, ft_complex &result);

void ft_sub(ft_complex &a, const ft_complex &b);

void ft_polar(double angle, ft_complex &result);
