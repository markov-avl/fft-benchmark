#pragma once

#include <cmath>
#include <cstring>

#include "types.h"

#define FT_COPY(__from, __to) \
    memcpy(&__to, &__from, 16)

#define FT_MUL(__a, __b, __result) \
    __result[0] = __a[0] * __b[0] - __a[1] * __b[1]; \
    __result[1] = __a[0] * __b[1] + __a[1] * __b[0]

#define FT_RMUL(__a, __b) { \
    const double __t = __a[0]; \
    __a[0] = __t * __b[0] - __a[1] * __b[1]; \
    __a[1] = __t * __b[1] + __a[1] * __b[0]; \
}

#define FT_ADD(__a, __b, __result) \
    __result[0] = __a[0] + __b[0]; \
    __result[1] = __a[1] + __b[1]

#define FT_RADD(__a, __b) \
    __a[0] += __b[0]; \
    __a[1] += __b[1]

#define FT_RSUB(__a, __b, __result) \
    __result[0] = __a[0] - __b[0]; \
    __result[1] = __a[1] - __b[1]

#define FT_SUB(__a, __b, __result) \
    __result[0] = __a[0] - __b[0]; \
    __result[1] = __a[1] - __b[1]

#define FT_POLAR(__angle, __result) \
    __result[0] = std::cos(__angle); \
    __result[1] = std::sin(__angle)
