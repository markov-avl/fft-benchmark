#pragma once

#include <cstddef>


void set_max_threads(std::size_t n);

std::size_t get_max_threads();

std::size_t get_system_threads();
