#include "tunes.h"

int getTone(int signal, int sensorIndex) {
  // For 2 octaves of 14 notes
  // ---------------------
  // | 0 < signal < 1024 |
  // | 0 <   note <   14 |
  // ---------------------
  // note = signal*notesSize/analogMax

  int noteIndex = floor(signal * TUNES_SIZE_LIST[sensorIndex] / ANALOG_MAX);
  double quotient = TUNES_LIST[sensorIndex][noteIndex];

  return round(pow(2., quotient - 1.) * TONE_A1);
}
