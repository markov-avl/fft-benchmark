#pragma once


enum SupportedSequences {
    ANY = 0,
    PRIME = 1 << 0,
    COPRIME = 1 << 1,
    RADIX_2 = 1 << 2,
    RADIX_3 = 1 << 3,
    RADIX_4 = 1 << 4,
    RADIX_5 = 1 << 5
};
