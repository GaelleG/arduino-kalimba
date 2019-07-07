/*
  K A L I M B A

  Plays a range of notes that change according to the analog input.

  Circuit:
  - one LDR
  - one 10 kilohm resistor from analog pin A0 through 5 to ground
  - one speaker on digital pin 9~

  created  5th Jully 2019
  modified 7th Jully 2019
  by Gaelle Gomez

  GNU GPLv3

  https://github.com/GaelleG/arduino-kalimba
*/

double frequency(int signal);
double arpediator(int signal);
double semitoneFrequency(int signal);
double semitoneRound(double tone);

const double ANALOG_MAX = 1024.;
const double TONE_A1 = 220.;
const double OCTAVE = 3.;

#define QUOTIENT_A   0./12.
#define QUOTIENT_AS  1./12.
#define QUOTIENT_B   2./12.
#define QUOTIENT_C   3./12.
#define QUOTIENT_CS  4./12.
#define QUOTIENT_D   5./12.
#define QUOTIENT_DS  6./12.
#define QUOTIENT_E   7./12.
#define QUOTIENT_F   8./12.
#define QUOTIENT_FS  9./12.
#define QUOTIENT_G  10./12.
#define QUOTIENT_GS 11./12.

const double QUOTIENTS[] = {
  QUOTIENT_A,
  QUOTIENT_AS,
  QUOTIENT_D,
  QUOTIENT_F,
  QUOTIENT_G
};
const int QUOTIENTS_SIZE = sizeof(QUOTIENTS) / sizeof(QUOTIENTS[0]);
const int NOTES_SIZE = OCTAVE * QUOTIENTS_SIZE;

double previousTone = 0.;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(A0);
  double frequencyValue = arpegiatorFrequency(sensorValue);

  if (frequencyValue != previousTone) {
    tone(9, frequencyValue, 100);
  }

  previousTone = frequencyValue;
}

double frequency(int signal) {
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

double arpegiatorFrequency(int signal) {
  // For 2 octaves of 7 notes
  // ---------------------
  // | 0 < signal < 1024 |
  // | 0 < octave <    2 |
  // | 0 <   note <   14 |
  // ---------------------
  // octave = signal*octaveSize/analogMax
  // note = signal*notesSize/analogMax

  int octave = floor(signal * OCTAVE / ANALOG_MAX);
  int note = ceil(signal * NOTES_SIZE / ANALOG_MAX);
  int quotientIndex = note - octave * QUOTIENTS_SIZE - 1;

  double quotient = octave + QUOTIENTS[quotientIndex];

  return pow(2., quotient - 1.) * TONE_A1;
}

double semitoneFrequency(int signal) {
  double x = signal * 2. / ANALOG_MAX;
  double rounded = semitoneRound(x);

  return pow(2., rounded - 1.) * TONE_A1;
}

double semitoneRound(double x) {
  double multiple = 12.;

  return multiple * floor(((x * 100. + multiple / 2.) / multiple)) / 100.;
}
