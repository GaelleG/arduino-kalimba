#include "notes.h"

int getTone(int signal, int sensorIndex);
void setFrequencies();
int getFrequency(double quotient);

const int VOICES_COUNT = 3;
const int NOTES_COUNT = 16;

const double VOICES[VOICES_COUNT][NOTES_COUNT] = {
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
  },
  {
    2. + 0. + QUOTIENT_A,
    2. + 0. + QUOTIENT_C,
    2. + 0. + QUOTIENT_E,
    2. + 0. + QUOTIENT_F,
    2. + 1. + QUOTIENT_A,
    2. + 1. + QUOTIENT_C,
    2. + 1. + QUOTIENT_E,
    2. + 1. + QUOTIENT_F,
    2. + 2. + QUOTIENT_A,
    2. + 2. + QUOTIENT_C,
    2. + 2. + QUOTIENT_E,
    2. + 2. + QUOTIENT_F,
    2. + 3. + QUOTIENT_A,
    2. + 3. + QUOTIENT_C,
    2. + 3. + QUOTIENT_E,
    2. + 3. + QUOTIENT_F
  }
};

int frequencies[VOICES_COUNT][NOTES_COUNT];

int getTone(int signal, int sensorIndex) {
  // For 2 octaves of 14 notes
  // ---------------------
  // | 0 < signal < 1024 |
  // | 0 <   note <   14 |
  // ---------------------
  // note = signal*notesSize/analogMax
  int noteIndex = floor(signal * NOTES_COUNT / ANALOG_MAX);

  return frequencies[sensorIndex][noteIndex];
}

void setFrequencies() {
  for (int i = 0; i < VOICES_COUNT; i++) {
    for (int j = 0; j < NOTES_COUNT; j++) {
      frequencies[i][j] = getFrequency(VOICES[i][j]);
    }
  }
}

int getFrequency(double quotient) {
  return round(pow(2., quotient - 1.) * TONE_A1);
}
