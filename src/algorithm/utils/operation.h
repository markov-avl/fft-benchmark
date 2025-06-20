#pragma once

#include <cmath>
#include <cstring>

#define FT_COPY(__from, __to) \
    __to[0] = __from[0]; \
    __to[1] = __from[1]

#define FT_ARRAY_COPY(__n, __from, __to) \
    memcpy(__to, __from, __n * 16)

#define FT_ZERO(__a) \
    __a[0] = 0.0; \
    __a[1] = 0.0

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

#define FT_CONJ(__result, __a) \
    __result[0] = __a[0]; \
    __result[1] = -__a[1]
