#pragma once


enum SupportedSequences {
    ANY = 0,
    PRIME = 1 << 0,
    COPRIME = 1 << 1,
    RADIX_2 = 1 << 2,
    RADIX_3 = 1 << 3,
    RADIX_4 = 1 << 4,
    RADIX_5 = 1 << 5,
    RADIX_6 = 1 << 6,
    RADIX_7 = 1 << 7,
    RADIX_8 = 1 << 8
};
