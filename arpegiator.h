#include "notes.h"

const double NOTES[] = {
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
};
const int NOTES_SIZE = sizeof(NOTES) / sizeof(NOTES[0]);
const int TONE_LIST_SIZE = 3;

double getTone(int signal) {
  // For 2 octaves of 14 notes
  // ---------------------
  // | 0 < signal < 1024 |
  // | 0 <   note <   14 |
  // ---------------------
  // note = signal*notesSize/analogMax

  int noteIndex = floor(signal * NOTES_SIZE / ANALOG_MAX);
  double quotient = NOTES[noteIndex];

  return pow(2., quotient - 1.) * TONE_A1;
}

void setToneList(int signal, double *list) {
  int noteIndex = floor(signal * NOTES_SIZE / ANALOG_MAX);
  list[0] = pow(2., NOTES[noteIndex] - 1.) * TONE_A1;

  for (int i = 1; i < TONE_LIST_SIZE; i++) {
    noteIndex = noteIndex > 0 ? noteIndex - 1 : 0;
    list[i] = pow(2., NOTES[noteIndex] - 1.) * TONE_A1;
  }
}
