#include "notes.h"

double getTone(int signal) {
  // ----------------------
  // |   A1 |   A2 |   A3 |
  // |  110 |  220 |  440 |
  // ----------------------
  // Ax = pow(2,x-1)*110

  // For 2 octaves A2 to A4
  // freq = pow(2, x-1)*220
  // --------------------------
  // |   220 <   freq <   880 |
  // |     0 <      x <     2 |
  // |     0 < signal <  1024 |
  // --------------------------
  // x = signal*2/1024

  double x = signal * 2. / ANALOG_MAX;

  return pow(2., x - 1.) * TONE_A1;
}
