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

void setToneList(int signal, double *list) {
  double x = signal * 2. / ANALOG_MAX;
  double rounded = semitoneRound(x);

  list[0] = pow(2., rounded - 1.) * TONE_A1;
  list[1] = pow(2., rounded - 1. / 12. - 1.) * TONE_A1;
  list[1] = pow(2., rounded - 2. / 12. - 1.) * TONE_A1;
}
