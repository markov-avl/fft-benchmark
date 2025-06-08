#pragma once


enum SupportedSequences {
    ANY = 0,
    PRIME = 1 << 0,
    COPRIME = 1 << 1,
    BASE_OF_2 = 1 << 2,
    BASE_OF_3 = 1 << 3,
    BASE_OF_4 = 1 << 4,
    BASE_OF_5 = 1 << 5
};
