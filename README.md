
# Arduino Kalimba ─ A noisy sampler   

[1. Idea](#1-idea)   
[2. What is a note?](#2-what-is-a-note)   
[3. Analogical into frequency](#3-analogical-into-frequency)   
[4. Configurable voices](#4-configurable-voices)   
[5. Circuit test](#5-circuit-test)   
[6. Handcrafted controllers](#6-handcrafted-controllers)   

## 1. Idea   

In July, I went to my first electronical music workshop.   
I brought an unused Arduino Starter Kit I got from a friend.   
I quickly got a buzzer sound from a light sensor analogical signal but I wanted to play real notes.   
There was a kalimba – also called mbira or even "thumbs piano". Thanks to its melodious tuning, whatever you do, it's pleasant.   

## 2. What is a note?   

A note is defined by its hertz frequency:   
- high frequency means high tone,
- low frequency means low tone.

The reference is the note A or "la":
  A0 is 55Hz, A1 is 110Hz, A2 is 220Hz, etc.

## 3. Analogical into frequency   

```
For 2 octaves from A1 to A3:   
  A0 →  55   
  A1 → 110   
  A2 → 220   
Ax = 55 * pow(2, x)   
```

```
Given 110 <   freq <  440
  and   0 <      x <    2
  and   0 < signal < 1024
Thus  x    = signal * 2 / 1024
  and freq = pow(2, x) * 110
e.g.  x    = 512 * 2 / 1024 = 1
  and freq = pow(2, 1) * 110 = 220
```
But actually C is so precise when it computes pows that it slows down everything.   

## 4. Configurable voices   

Features:
- global on off
- metronome
- as many voices as you want
- voices' pretuned notes list
- voices' 8 beats measure loop
- voices' play and stop

For each voice:
- note selector: 10k pot
- recorder: button, red LED, 220Ω resistor
- play/stop: button, blue LED, 10kΩ resistor

## 5. Circuit test   

```
    ┌────────┬┬┬┬┬┬┬┐ ┌───────┬┬┬┬┬┬┬┬┬┬┐ ┌─────────┐
    │ ON OFF ││││││││ │ TEMPO │││││││││││ │ SPEAKER │
    └────────┴┴┴┴┴┴┴┘ └───────┴┴┴┴┴┴┴┴┴┴┘ └─────────┘
   D13       -        +    A0   -        D11~       -
    │        │        │    │    │         │         │
    └ SWITCH ┘        └ POT 10k ┘         └ SPEAKER ┘

    ┌─────────┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┐
    │ VOICE 1 │││││││││││││││││││││││││││││││││││││││
    └─────────┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┘
    ┌────────┐    ┌─────────┐    ┌──────────────────┐
    │ RECORD │    │  TONE   │    │      ON OFF      │
    └────────┘    └─────────┘    └──────────────────┘
    +    D2  -    +    A3   -    D3       -    D4   -
    │    │   │    │    │    │    │        │    │    │
    │    │  RED   │    │    │    │        │   RES  BLUE
    │    │  LED   │    │    │    │        │   10k  LED
    │    │   │    │    │    │    │        │    │    │
    │    │  RES   │    │    │    │        │    └────┘
    │    │  220   │    │    │    │        │
    │    └───┤    │    │    │    │        │
    └ SWITCH ┘    └ POT 10k ┘    └ SWITCH ┘

    ┌─────────┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┐
    │ VOICE 2 │││││││││││││││││││││││││││││││││││││││
    └─────────┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┘
    ┌────────┐    ┌─────────┐    ┌──────────────────┐
    │ RECORD │    │  TONE   │    │      ON OFF      │
    └────────┘    └─────────┘    └──────────────────┘
    +    D4  -    +    A2   -    D6       -    D7   -
    │    │   │    │    │    │    │        │    │    │
    │    │  RED   │    │    │    │        │   RES  BLUE
    │    │  LED   │    │    │    │        │   10k  LED
    │    │   │    │    │    │    │        │    │    │
    │    │  RES   │    │    │    │        │    └────┘
    │    │  220   │    │    │    │        │
    │    └───┤    │    │    │    │        │
    └ SWITCH ┘    └ POT 10k ┘    └ SWITCH ┘

    ┌─────────┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┬┐
    │ VOICE 3 │││││││││││││││││││││││││││││││││││││││
    └─────────┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┴┘
    ┌────────┐    ┌─────────┐    ┌──────────────────┐
    │ RECORD │    │  TONE   │    │      ON OFF      │
    └────────┘    └─────────┘    └──────────────────┘
    +    D8  -    +    A1   -    D9       -   D10   -
    │    │   │    │    │    │    │        │    │    │
    │    │  RED   │    │    │    │        │   RES  BLUE
    │    │  LED   │    │    │    │        │   10k  LED
    │    │   │    │    │    │    │        │    │    │
    │    │  RES   │    │    │    │        │    └────┘
    │    │  220   │    │    │    │        │
    │    └───┤    │    │    │    │        │
    └ SWITCH ┘    └ POT 10k ┘    └ SWITCH ┘
```

## 6. Handcrafted controllers   

Material from trash:
- shipping foam from office trash
- cardboard from breakfast cereal
- electronic circuit leftover
- copper from an electrical cable leftover

Arduino Starter Kit components:
- LEDs, buttons, potentiometers, resistors and rigid jumpers

Alternative use:
- drawing pin, binder clips and staples

Bought for this project:
- dupont ribbon jumpers

Issues:
- copper works really well but it's long to set up
- connections with staple are not reliable
- controllers are too light to support ribbon pulling

But the esthetic is really nice so maybe I will explore some other handcrafted methods.
