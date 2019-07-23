/*
  K A L I M B A

  Plays a range of notes that change according to the analog input.

  Circuit:
  - one potentiometer 10k from analog pin A0 through 5 to ground
  - one potentiometer 10k from analog pin A1 through 5 to ground
  - one potentiometer 10k from analog pin A2 through 5 to ground
  - one speaker on digital pin 9~

  created  5th  Jully 2019
  modified 23th Jully 2019
  by Gaelle Gomez

  GNU GPLv3

  https://github.com/GaelleG/arduino-kalimba
*/

#include "arpegiator.h"
// #include "frequencies.h"
// #include "semitones.h"

struct Sensor {
  int pin;
  int value;
  int previousValue;
  int convertedValue;
  int previousConvertedValue;
};

struct Sensor sensorList[2] = {
  {A0, 0, 0, 0, 0},
  {A1, 0, 0, 0, 0}
};

Sensor *sensor;

const unsigned long MINUTE = 60000;
unsigned long time = millis();
unsigned long previousTime = millis();
unsigned long elapsedTime = 0.;

struct Sensor tempoSensor = {A2, 0, 0, 0, 0};

unsigned long timeList[2] = {0., 0.};

void setTempo();
void toneEcho(unsigned long elapsedTime);
void toneSensorFrequency();

void setup() {
  Serial.begin(9600);
}

void loop() {
  time = millis();
  elapsedTime = time - previousTime;
  previousTime = time;

  setTempo();
  toneEcho(elapsedTime);
  toneSensorFrequency();
}

void setTempo() {
  // From 30bpm to 210bpm
  // ----------------------
  // |  0 < signal < 1024 |
  // | 60 <  tempo <  210 |
  // ----------------------
  // tempo = 60 + signal * (210 - 60) / 1024
  tempoSensor.value = analogRead(tempoSensor.pin);

  if (abs(tempoSensor.value - tempoSensor.previousValue) > 10) {
    tempoSensor.previousValue = tempoSensor.value;
    tempoSensor.previousConvertedValue = tempoSensor.convertedValue;
    tempoSensor.convertedValue = MINUTE / (60. + tempoSensor.value * (210. - 60.) / 1024. + 1.);

    timeList[1] = timeList[0] + tempoSensor.convertedValue / 2.;
  }
}

void toneEcho(unsigned long elapsedTime) {
  for (int i = 0; i < 2; i++) {
    timeList[i] += elapsedTime;
    if (timeList[i] > tempoSensor.convertedValue) {
      timeList[i] -= tempoSensor.convertedValue;
      sensor = &sensorList[i];
      tone(9, sensor->previousConvertedValue, 20);
    }
  }
}

void toneSensorFrequency() {
  for (int i = 0; i < 2; i++) {
    sensor = &sensorList[i];
    sensor->value = analogRead(sensor->pin);
    sensor->convertedValue = getTone(sensor->value, i);

    if (sensor->convertedValue != sensor->previousConvertedValue) {
      tone(9, sensor->convertedValue, 50);
      sensor->previousConvertedValue = sensor->convertedValue;
    }
  }
}
