/*
  K A L I M B A

  Plays a range of notes that change according to the analog input.

  Circuit:
  - one potentiometer 10k from analog pin A0 through 5 to ground
  - one potentiometer 10k from analog pin A1 through 5 to ground
  - one speaker on digital pin 9~

  created  5th Jully 2019
  modified 22th Jully 2019
  by Gaelle Gomez

  GNU GPLv3

  https://github.com/GaelleG/arduino-kalimba
*/

#define MODE_SIMPLE_TONE 1
#define MODE_MULTI_TONE  2

#include "arpegiator.h"
// #include "frequencies.h"
// #include "semitones.h"

struct Sensor {
  int pin;
  int value;
  int frequency;
  int previousFrequency;
};

struct Sensor sensorList[2] = {
  {A0, 0, 0, 0},
  {A1, 0, 0, 0}
};

Sensor *sensor;

const unsigned long MINUTE = 60000;
unsigned long time = millis();
unsigned long previousTime = millis();
unsigned long elapsedTime = 0.;
unsigned long tempo = MINUTE / 120.;
unsigned long timeList[2] = {0., tempo / 2.};

void toneEcho(unsigned long elapsedTime);
void toneSensorFrequency();

void setup() {
  Serial.begin(9600);
}

void loop() {
  time = millis();
  elapsedTime = time - previousTime;
  previousTime = time;
  toneEcho(elapsedTime);
  toneSensorFrequency();
}

void toneEcho(unsigned long elapsedTime) {
  for (int i = 0; i < 2; i++) {
    timeList[i] += elapsedTime;
    if (timeList[i] > tempo) {
      timeList[i] -= tempo;
      sensor = &sensorList[i];
      tone(9, sensor->previousFrequency, 20);
    }
  }
}

void toneSensorFrequency() {
  for (int i = 0; i < 2; i++) {
    sensor = &sensorList[i];
    sensor->value = analogRead(sensor->pin);
    sensor->frequency = getTone(sensor->value, i);

    if (sensor->frequency != sensor->previousFrequency) {
      tone(9, sensor->frequency, 50);
      sensor->previousFrequency = sensor->frequency;
    }
  }
}
