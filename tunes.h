#include "notes.h"

const double TUNES_LIST[2][16] = {
  {
    0. + QUOTIENT_D,
    0. + QUOTIENT_F,
    1. + QUOTIENT_A,
    1. + QUOTIENT_C,
    1. + QUOTIENT_D,
    1. + QUOTIENT_F,
    2. + QUOTIENT_A,
    2. + QUOTIENT_C,
    2. + QUOTIENT_D,
    2. + QUOTIENT_F,
    3. + QUOTIENT_A,
    3. + QUOTIENT_C,
    3. + QUOTIENT_D,
    3. + QUOTIENT_F,
    4. + QUOTIENT_A,
    4. + QUOTIENT_C
  },
  {
    1. + 0. + QUOTIENT_F,
    1. + 1. + QUOTIENT_A,
    1. + 1. + QUOTIENT_C,
    1. + 1. + QUOTIENT_E,
    1. + 1. + QUOTIENT_F,
    1. + 2. + QUOTIENT_A,
    1. + 2. + QUOTIENT_C,
    1. + 2. + QUOTIENT_E,
    1. + 2. + QUOTIENT_F,
    1. + 3. + QUOTIENT_A,
    1. + 3. + QUOTIENT_C,
    1. + 3. + QUOTIENT_E,
    1. + 3. + QUOTIENT_F,
    1. + 4. + QUOTIENT_A,
    1. + 4. + QUOTIENT_C,
    1. + 4. + QUOTIENT_E
  }
};

const int TUNES_SIZE_LIST[2] = {
  sizeof(TUNES_LIST[0]) / sizeof(double),
  sizeof(TUNES_LIST[1]) / sizeof(double)
};

const int TONE_LIST_SIZE = 3;
