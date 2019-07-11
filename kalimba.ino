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

#include "arpegiator.h"
// #include "frequencies.h"
// #include "semitones.h"

double previousTone = 0.;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(A0);
  double frequency = getTone(sensorValue);
  //double frequencyList[TONE_LIST_SIZE];
  //setToneList(sensorValue, frequencyList);

  if (frequency != previousTone) {
    tone(9, frequency, 50);
    //toneFrequencyList(frequencyList);
  }

  previousTone = frequency;
}

void toneFrequencyList(double* frequencyList) {
  for (int i = 0; i < TONE_LIST_SIZE; i++) {
    tone(9, frequencyList[i], 50);
    if (i < TONE_LIST_SIZE - 1) {
      delay(50);
    }
  }
}
