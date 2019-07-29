/*
  K A L I M B A

  Plays a range of notes that change according to the analog input.

  Circuit:
  - one potentiometer 10k from analog pin through 5 to ground
  - one potentiometer 10k from analog pin through 5 to ground
  - one potentiometer 10k from analog pin through 5 to ground
  - one potentiometer 10k from analog pin through 5 to ground
  - one switch from 5 through 220 resistor to digital pin
  - one speaker on digital pin ~

  created  5th  July 2019
  modified 28th July 2019
  by Gaelle Gomez

  GNU GPLv3

  https://github.com/GaelleG/arduino-kalimba
*/

#include "arpegiator.h"
// #include "frequencies.h"
// #include "semitones.h"

const int STATE_PIN = 13;
const int TEMPO_PIN_1 = A0;
const int VOICE_PIN_1 = A1;
const int VOICE_PIN_2 = A2;
const int VOICE_PIN_3 = A3;
const int PIEZO_PIN = 3;

struct Sensor {
  int pin;
  int value;
  int previousValue;
  int convertedValue;
  int previousConvertedValue;
  int lastValueUpdate;
};

struct Sensor sensorList[VOICES_COUNT] = {
  {VOICE_PIN_1, 0, 0, 0, 0, 0},
  {VOICE_PIN_2, 0, 0, 0, 0, 0},
  {VOICE_PIN_3, 0, 0, 0, 0, 0}
};

Sensor *sensor;

const unsigned long MINUTE = 60000;
unsigned long time = millis();
unsigned long previousTime = millis();
unsigned long elapsedTime = 0.;

struct Sensor tempoSensor = {TEMPO_PIN_1, 0, 0, 0, 0, 0};

unsigned long timeList[VOICES_COUNT] = {0.};

const int STATE_RUNNING = 1;
const int STATE_STOPPED = -1;
struct Sensor stateSensor = {STATE_PIN, 0, LOW, STATE_STOPPED, STATE_STOPPED, 0};

void setState();
void setTempo();
void toneEcho();
void toneSensorFrequency();
bool hysteresis(Sensor* currentSensor);

void setup() {
  Serial.begin(9600);
  pinMode(stateSensor.pin, INPUT);
  setFrequencies();
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

    double tempoGap = tempoSensor.convertedValue / VOICES_COUNT;
    for (int i = 1; i < VOICES_COUNT; i++) {
      timeList[i] = timeList[i - 1] + tempoGap;
    }
  }
}

void toneEcho() {
  for (int i = 0; i < VOICES_COUNT; i++) {
    timeList[i] += elapsedTime;
    if (timeList[i] > tempoSensor.convertedValue) {
      timeList[i] -= tempoSensor.convertedValue;
      sensor = &sensorList[i];
      tone(PIEZO_PIN, sensor->previousConvertedValue, 20);
    }
  }
}

void toneSensorFrequency() {
  for (int i = 0; i < VOICES_COUNT; i++) {
    sensor = &sensorList[i];
    sensor->value = analogRead(sensor->pin);

    if (hysteresis(sensor)) {
      sensor->lastValueUpdate = time;
      sensor->previousValue = sensor->value;
      sensor->convertedValue = getTone(sensor->value, i);
      sensor->previousConvertedValue = sensor->convertedValue;

      tone(PIEZO_PIN, sensor->convertedValue, 50);
    }
  }
}

bool hysteresis(Sensor* currentSensor) {
  return (time - currentSensor->lastValueUpdate > 100 && abs(currentSensor->value - currentSensor->previousValue) > 4);
}
