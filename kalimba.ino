/*
  K A L I M B A

  Plays a range of notes that change according to the analog input.

  Circuit:
  - 1 speaker
  - 1 push button ON OFF
  - 1 potentiometer 10k TEMPO
  - per voice:
    - 1 potentiometers 10k
    - 1 red LED
    - 1 blue LED
    - 1 220 resistor
    - 2 push buttons

  created  2019-07-05
  modified 2019-09-28
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

struct Voice {
  Sensor frequencySensor;
  Sensor onOffSensor;
  Sensor recordSensor;
  int pinOnOffLED;
  unsigned long timeMeasure;
};

// -------------------------------------------------------------------- PIN LIST
// ········································································ READ
const int PIN_ON_OFF = 13;
const int PIN_TEMPO = A0;
// ······································································· WRITE
const int PIN_SPEAKER = 11;

// ----------------------------------------------------------------- ACTION LIST
// ······································································ ON OFF
const int ON = 1;
const int OFF = -1;
struct Sensor onOffSensor = {PIN_ON_OFF, 0, LOW, OFF, OFF, 0};
// ······································································ VOICES
const int PIN_TONE_LIST[VOICES_COUNT] = {A3, A2, A1};
const int PIN_ON_OFF_LIST[VOICES_COUNT] = {3, 6, 9};
const int PIN_RECORD_LIST[VOICES_COUNT] = {2, 5, 8};
const int PIN_ON_OFF_LED_LIST[VOICES_COUNT] = {4, 7, 10};
struct Voice voiceList[VOICES_COUNT] = {};
// ······································································· TEMPO
struct Sensor tempoSensor = {PIN_TEMPO, 0, 0, 0, 0, 0};
unsigned long tempoTime = 0.;

// ------------------------------------------------------------------------ TIME
const unsigned long MINUTE = 60000;
unsigned long time = millis();
unsigned long previousTime = millis();
unsigned long elapsedTime = 0.;

Sensor *sensor;

void setup() {
  Serial.begin(9600);
  pinMode(onOffSensor.pin, INPUT_PULLUP);
  setFrequencies();

  for (int i = 0; i < VOICES_COUNT; ++i) {
    pinMode(PIN_ON_OFF_LIST[i], INPUT_PULLUP);
    pinMode(PIN_RECORD_LIST[i], INPUT_PULLUP);

    voiceList[i] = {
      {PIN_TONE_LIST[i], 0, 0, 0, 0, 0},
      {PIN_ON_OFF_LIST[i], 0, LOW, OFF, OFF, 0},
      {PIN_RECORD_LIST[i], 0, LOW, OFF, OFF, 0},
      PIN_ON_OFF_LED_LIST[i],
      0.
    };
  }
}

void loop() {
  time = millis();
  elapsedTime = time - previousTime;
  previousTime = time;

  setState();

  if (onOffSensor.convertedValue == ON) {
    setOnOffState();
    setTempo();
    toneEcho();
    toneSensorFrequency();
  }
}

void setState() {
  if (setPushButton(&onOffSensor)) {
    int value = onOffSensor.convertedValue;
    for (int i = 0; i < VOICES_COUNT; ++i) {
      voiceList[i].onOffSensor.convertedValue = value;
      voiceList[i].onOffSensor.previousConvertedValue = value;
    }
    setOnOffLED();
  }
}

void setOnOffState() {
  for (int i = 0; i < VOICES_COUNT; ++i) {
    if (setPushButton(&(voiceList[i].onOffSensor))) {
      if (voiceList[i].onOffSensor.convertedValue == HIGH) {
        digitalWrite(voiceList[i].pinOnOffLED, 1);
      } else {
        digitalWrite(voiceList[i].pinOnOffLED, 0);
      }
    }
  }
}

void setOnOffLED() {
  for (int i = 0; i < VOICES_COUNT; ++i) {
    if (voiceList[i].onOffSensor.convertedValue == HIGH) {
      digitalWrite(voiceList[i].pinOnOffLED, 1);
    } else {
      digitalWrite(voiceList[i].pinOnOffLED, 0);
    }
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
      voiceList[i].timeMeasure = voiceList[i - 1].timeMeasure + tempoGap;
    }
  }
}

void toneEcho() {
  for (int i = 0; i < VOICES_COUNT; i++) {
    voiceList[i].timeMeasure += elapsedTime;
    if (voiceList[i].timeMeasure > tempoSensor.convertedValue) {
      voiceList[i].timeMeasure -= tempoSensor.convertedValue;
      if (voiceList[i].onOffSensor.convertedValue == ON) {
        sensor = &(voiceList[i].frequencySensor);
        tone(PIN_SPEAKER, sensor->previousConvertedValue, 20);
      }
    }
  }
}

void toneSensorFrequency() {
  for (int i = 0; i < VOICES_COUNT; i++) {
    sensor = &(voiceList[i].frequencySensor);
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
