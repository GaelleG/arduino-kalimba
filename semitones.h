#include "notes.h"

double semitoneRound(double x);

double getTone(int signal) {
  double x = signal * 2. / ANALOG_MAX;
  double rounded = semitoneRound(x);

  return pow(2., rounded - 1.) * TONE_A1;
}

double semitoneRound(double x) {
  double multiple = 12.;

  return multiple * floor(((x * 100. + multiple / 2.) / multiple)) / 100.;
}
