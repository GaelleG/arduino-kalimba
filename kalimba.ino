/*
  K A L I M B A

  Plays a range of notes that change according to the analog input.

  Circuit:
  - one potentiometer 10k from analog pin A0 through 5 to ground
  - one potentiometer 10k from analog pin A1 through 5 to ground
  - one potentiometer 10k from analog pin A2 through 5 to ground
  - one speaker on digital pin 9~
  - one switch from 5 through 220 resistor to pin 2

  created  5th  July 2019
  modified 28th July 2019
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
  int lastValueUpdate;
};

struct Sensor sensorList[2] = {
  {A0, 0, 0, 0, 0, 0},
  {A1, 0, 0, 0, 0, 0}
};

Sensor *sensor;

const unsigned long MINUTE = 60000;
unsigned long time = millis();
unsigned long previousTime = millis();
unsigned long elapsedTime = 0.;

struct Sensor tempoSensor = {A2, 0, 0, 0, 0, 0};

unsigned long timeList[2] = {0., 0.};

const int STATE_RUNNING = 1;
const int STATE_STOPPED = -1;
struct Sensor stateSensor = {2, 0, LOW, STATE_STOPPED, STATE_STOPPED, 0};

void setState();
void setTempo();
void toneEcho();
void toneSensorFrequency();
bool hysteresis(Sensor* currentSensor);

void setup() {
  Serial.begin(9600);
  pinMode(stateSensor.pin, INPUT);
}

void loop() {
  time = millis();
  elapsedTime = time - previousTime;
  previousTime = time;

  setState();

  if (stateSensor.convertedValue == STATE_RUNNING) {
    setTempo();
    toneEcho();
    toneSensorFrequency();
  }
}

void setState() {
  stateSensor.value = digitalRead(stateSensor.pin);

  if (time - stateSensor.lastValueUpdate > 500 && stateSensor.value == HIGH && stateSensor.previousValue != stateSensor.value) {
    stateSensor.convertedValue *= -1;
    stateSensor.previousConvertedValue = stateSensor.convertedValue;
    stateSensor.lastValueUpdate = time;
  }

  stateSensor.previousValue = stateSensor.value;
}

void setTempo() {
  // From 60bpm to 210bpm
  // ----------------------
  // |  0 < signal < 1024 |
  // | 60 <  tempo <  210 |
  // ----------------------
  // tempo = 60 + signal * (210 - 60) / 1024
  tempoSensor.value = analogRead(tempoSensor.pin);

  if (hysteresis(&tempoSensor)) {
    tempoSensor.lastValueUpdate = time;
    tempoSensor.previousValue = tempoSensor.value;
    tempoSensor.previousConvertedValue = tempoSensor.convertedValue;
    tempoSensor.convertedValue = MINUTE / (60. + tempoSensor.value * (210. - 60.) / 1024. + 1.);

    timeList[1] = timeList[0] + tempoSensor.convertedValue / 2.;
  }
}

void toneEcho() {
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

    if (hysteresis(sensor)) {
      sensor->lastValueUpdate = time;
      sensor->previousValue = sensor->value;
      sensor->convertedValue = getTone(sensor->value, i);
      sensor->previousConvertedValue = sensor->convertedValue;

      tone(9, sensor->convertedValue, 50);
    }
  }
}

bool hysteresis(Sensor* currentSensor) {
  return (time - currentSensor->lastValueUpdate > 100 && abs(currentSensor->value - currentSensor->previousValue) > 4);
}
