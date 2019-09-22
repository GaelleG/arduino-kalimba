/*
  K A L I M B A

  Plays a range of notes that change according to the analog input.

  Circuit:
  - 4 potentiometers 10k
  - 4 push buttons
  - 4 220 resistors
  - 3 LEDs
  - 3 10k resistors
  - 1 speaker

  created  2019-07-05
  modified 2019-09-22
  by Gaelle Gomez

  GNU GPLv3

  https://github.com/GaelleG/arduino-kalimba
*/

#include "arpegiator.h"

struct Sensor {
  int pin;
  int value;
  int previousValue;
  int convertedValue;
  int previousConvertedValue;
  int lastValueUpdate;
};

// ==================================================================== PIN LIST
// ------------------------------------------------------------------------ read
const int PIN_ON_OFF = 13;
const int PIN_RECORD_LIST[VOICES_COUNT] = {12, 11, 10};
const int PIN_TONE_LIST[VOICES_COUNT] = {A1, A2, A3};
const int PIN_TEMPO = A0;
// ----------------------------------------------------------------------- write
const int PIN_SPEAKER = 3;
const int PIN_RECORD_LED_LIST[VOICES_COUNT] = {7, 6, 5};

// ================================================================= ACTION LIST
// ---------------------------------------------------------------------- ON OFF
const int ON = 1;
const int OFF = -1;
struct Sensor onOffSensor = {PIN_ON_OFF, 0, LOW, OFF, OFF, 0};
// ---------------------------------------------------------------------- VOICES
// ······································································ record
struct Sensor recordSensorList[VOICES_COUNT] = {
  {PIN_RECORD_LIST[0], 0, LOW, OFF, OFF, 0},
  {PIN_RECORD_LIST[1], 0, LOW, OFF, OFF, 0},
  {PIN_RECORD_LIST[2], 0, LOW, OFF, OFF, 0}
};
// ······································································· tones
struct Sensor toneSensorList[VOICES_COUNT] = {
  {PIN_TONE_LIST[0], 0, 0, 0, 0, 0},
  {PIN_TONE_LIST[1], 0, 0, 0, 0, 0},
  {PIN_TONE_LIST[2], 0, 0, 0, 0, 0}
};
// ····································································· measure
unsigned long voiceTimeList[VOICES_COUNT] = {0.};
// ----------------------------------------------------------------------- TEMPO
struct Sensor tempoSensor = {PIN_TEMPO, 0, 0, 0, 0, 0};
unsigned long tempoTime = 0.;

// ======================================================================== TIME
const unsigned long MINUTE = 60000;
unsigned long time = millis();
unsigned long previousTime = millis();
unsigned long elapsedTime = 0.;

Sensor *sensor;

void setup() {
  Serial.begin(9600);
  pinMode(onOffSensor.pin, INPUT);
  setFrequencies();
}

void loop() {
  time = millis();
  elapsedTime = time - previousTime;
  previousTime = time;

  setState();

  if (onOffSensor.convertedValue == ON) {
    setRecorderState();
    setTempo();
    toneEcho();
    toneSensorFrequency();
  }
}

void setState() {
  if (setPushButton(&onOffSensor)) {
    for (int i = 0; i < VOICES_COUNT; ++i) {
      recordSensorList[i].convertedValue = onOffSensor.convertedValue;
      recordSensorList[i].previousConvertedValue = recordSensorList[i].convertedValue;
    }
  }
}

void setRecorderState() {
  for (int i = 0; i < VOICES_COUNT; ++i) {
    setPushButton(&recordSensorList[i]);
  }
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
    tempoSensor.convertedValue = MINUTE /
      (60. + tempoSensor.value * (210. - 60.) / 1024. + 1.);

    double tempoGap = tempoSensor.convertedValue / VOICES_COUNT;
    for (int i = 1; i < VOICES_COUNT; i++) {
      voiceTimeList[i] = voiceTimeList[i - 1] + tempoGap;
    }
  }
}

void toneEcho() {
  for (int i = 0; i < VOICES_COUNT; i++) {
    voiceTimeList[i] += elapsedTime;
    if (voiceTimeList[i] > tempoSensor.convertedValue) {
      voiceTimeList[i] -= tempoSensor.convertedValue;
      if (recordSensorList[i].convertedValue == ON) {
        sensor = &toneSensorList[i];
        tone(PIN_SPEAKER, sensor->previousConvertedValue, 20);
        for (int j = 0; j < VOICES_COUNT; j++) {
          if (j == i) {
            digitalWrite(PIN_RECORD_LED_LIST[j], 1);
          } else {
            digitalWrite(PIN_RECORD_LED_LIST[j], 0);
          }
        }
      }
    }
  }
}

void toneSensorFrequency() {
  for (int i = 0; i < VOICES_COUNT; i++) {
    sensor = &toneSensorList[i];
    sensor->value = analogRead(sensor->pin);

    if (hysteresis(sensor)) {
      sensor->lastValueUpdate = time;
      sensor->previousValue = sensor->value;
      sensor->convertedValue = getTone(sensor->value, i);
      sensor->previousConvertedValue = sensor->convertedValue;

      tone(PIN_SPEAKER, sensor->convertedValue, 50);
    }
  }
}

bool setPushButton(Sensor* pushButton) {
  bool hasChanged = false;
  pushButton->value = digitalRead(pushButton->pin);

  if (time - pushButton->lastValueUpdate > 500 &&
      pushButton->value == HIGH &&
      pushButton->previousValue != pushButton->value) {
    pushButton->convertedValue *= -1;
    pushButton->previousConvertedValue = pushButton->convertedValue;
    pushButton->lastValueUpdate = time;
    hasChanged = true;
  }

  pushButton->previousValue = pushButton->value;

  return hasChanged;
}

bool hysteresis(Sensor* currentSensor) {
  return time - currentSensor->lastValueUpdate > 100 &&
         abs(currentSensor->value - currentSensor->previousValue) > 4;
}
