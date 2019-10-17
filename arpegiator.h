#include "notes.h"

void setFrequencies();
int getFrequency(double quotient);
int getTone(int signal, int sensorIndex);

const int VOICES_COUNT = 3;
const int NOTES_COUNT = 7 * 3;

const double VOICES[VOICES_COUNT][NOTES_COUNT] = {
  {
    0. + QUOTIENT_C,
    0. + QUOTIENT_D,
    0. + QUOTIENT_DS,
    0. + QUOTIENT_F,
    0. + QUOTIENT_G,
    0. + QUOTIENT_GS,
    1. + QUOTIENT_AS,
    1. + QUOTIENT_C,
    1. + QUOTIENT_D,
    1. + QUOTIENT_DS,
    1. + QUOTIENT_F,
    1. + QUOTIENT_G,
    1. + QUOTIENT_GS,
    2. + QUOTIENT_AS,
    2. + QUOTIENT_C,
    2. + QUOTIENT_D,
    2. + QUOTIENT_DS,
    2. + QUOTIENT_F,
    2. + QUOTIENT_G,
    2. + QUOTIENT_GS,
    3. + QUOTIENT_AS
  },
  {
    0. + QUOTIENT_DS,
    0. + QUOTIENT_F,
    0. + QUOTIENT_G,
    0. + QUOTIENT_GS,
    1. + QUOTIENT_AS,
    1. + QUOTIENT_C,
    1. + QUOTIENT_D,
    1. + QUOTIENT_DS,
    1. + QUOTIENT_F,
    1. + QUOTIENT_G,
    1. + QUOTIENT_GS,
    2. + QUOTIENT_AS,
    2. + QUOTIENT_C,
    2. + QUOTIENT_D,
    2. + QUOTIENT_DS,
    2. + QUOTIENT_F,
    2. + QUOTIENT_G,
    2. + QUOTIENT_GS,
    3. + QUOTIENT_AS,
    3. + QUOTIENT_C,
    3. + QUOTIENT_D
  },
  {
    0. + QUOTIENT_G,
    0. + QUOTIENT_GS,
    1. + QUOTIENT_AS,
    1. + QUOTIENT_C,
    1. + QUOTIENT_D,
    1. + QUOTIENT_DS,
    1. + QUOTIENT_F,
    1. + QUOTIENT_G,
    1. + QUOTIENT_GS,
    2. + QUOTIENT_AS,
    2. + QUOTIENT_C,
    2. + QUOTIENT_D,
    2. + QUOTIENT_DS,
    2. + QUOTIENT_F,
    2. + QUOTIENT_G,
    2. + QUOTIENT_GS,
    3. + QUOTIENT_AS,
    3. + QUOTIENT_C,
    3. + QUOTIENT_D,
    3. + QUOTIENT_DS,
    3. + QUOTIENT_F
  }
};

int frequencies[VOICES_COUNT][NOTES_COUNT];

void setFrequencies() {
  for (int i = 0; i < VOICES_COUNT; i++) {
    for (int j = 0; j < NOTES_COUNT; j++) {
      frequencies[i][j] = getFrequency(VOICES[i][j]);
    }
  }
}

int getFrequency(double quotient) {
  // The reference is the note A:
  //   A0 →  55Hz
  //   A1 → 110Hz
  //   A2 → 220Hz
  // Ax = 55 * pow(2, x)
  //
  // Given that an octave contains 12 notes
  // Thus  0 <= quotient < 12
  //   and x    = octave + quotient
  //   and freq = pow(2, x) * 55
  // e.g.  x    = 1
  //   and freq = pow(2, 1) * 55 = 110 = A1
  // e.g.  x    = 2 + 3 / 12
  //   and freq = pow(2, 2 + 3 / 12) * 55 = 261,6256 = C2

  return round(pow(2., quotient) * TONE_A0);
}

int getTone(int signal, int sensorIndex) {
  // For 2 octaves of 14 notes:
  //   0 < signal < 1024
  //   0 <   note <   14
  // Thus note = signal * notesSize / analogMax
  int noteIndex = floor(signal * NOTES_COUNT / ANALOG_MAX);

  return frequencies[sensorIndex][noteIndex];
}
