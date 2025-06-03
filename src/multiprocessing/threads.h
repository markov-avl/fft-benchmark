#pragma once

#include <cstddef>


void set_max_threads(size_t n);

size_t get_max_threads();

size_t get_system_threads();
