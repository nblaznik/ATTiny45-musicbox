'''
# ATtiny45 Music Box

This repository contains a small electronics project: a standalone music box based on an ATtiny45 and a passive piezo buzzer.

The project evolved from an Arduino prototype into a bare-metal ATtiny implementation.

---

## Repository structure

- ArduinoISP/  
  Arduino sketch used to turn an Arduino Uno/Nano into an ISP programmer for the ATtiny.

- buzzer/  
  Arduino-based buzzer experiments and melody prototyping using tone().

- buzzer_attiny/  
  Final ATtiny45 firmware.  
  Bare-metal AVR C code that generates square-wave audio using Timer0 and plays a melody on startup.

---

## How it works

- The ATtiny45 runs on its internal 1 MHz oscillator.
- Timer0 in CTC mode is used to generate square waves.
- An interrupt toggles an output pin connected to a passive piezo buzzer.
- Notes and durations are stored in flash (PROGMEM) to fit within the ATtiny’s limited SRAM.
- The melody plays automatically when power is applied.
- A physical power switch cuts battery power completely when off.

---

## Hardware overview

- ATtiny45
- Passive piezo buzzer
- 0.1 µF ceramic decoupling capacitor (VCC–GND)
- Battery (I used CR2032)
- Optional power switch (I used a mechnism from an old singing postcard, such that it starts playing when the music box is opened)

'''
Wiring:  
                          _________
                         |  notch  |
                    PB5  |1       8|  VCC -> [ Power Switch ] -> Battery+ 
                    PB3  |2       7|  PB2
                    PB4  |3       6|  PB1
Battery-, buzzer <- GND  |4       5|  PB0 -> buzzer output
                         |_________|
'''
---

## Programming

- Arduino IDE with ATTinyCore (Spence Konde)
- Arduino Uno/Nano used as Arduino as ISP
- Clock: 1 MHz internal
- Brown-out detection: disabled

---

## Notes

- Designed for passive piezo buzzers only.
- Volume depends on supply voltage, can control it with anm additional resistor in series. 
- Intended as a simple, low-power embedded music box. Simple gift. The music you need to write yourself, but I think there's online resources that can help with that. 
