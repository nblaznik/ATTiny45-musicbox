#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>


// ---------- pin ----------
#define BUZZER_PIN PB0

// ---------- notes ----------
#define REST 0
#define C5  523
#define D5  587
#define E5  659
#define F5  698
#define G5  784

// ---------- pitch scale ----------
#define PITCH_NUM 8
#define PITCH_DEN 5

// ---------- melody ----------
const uint16_t melody[] PROGMEM = {
  // verse
  REST, C5, C5, C5, D5, E5,
  REST, C5, C5, D5, F5, E5, D5, C5,
  REST, C5, C5, C5, D5, E5,
  REST, C5, C5, D5, F5, E5, D5, C5,
  REST, C5, C5, C5, D5, E5,
  REST, C5, C5, D5, F5, E5, D5, C5,
  REST, D5, D5, D5, E5, F5,
  REST, D5, D5, E5, G5, F5, E5, D5,
  // chorus
  C5, C5, C5, D5, D5, D5, C5,
  E5, E5, E5, E5, C5, D5, C5,
  C5, C5, C5, D5, D5, D5, C5,
  E5, E5, E5, E5, C5, D5, C5,
  C5, C5, C5, D5, D5, D5, C5,
  F5, F5, F5, F5, F5, E5, E5,
  D5, F5, F5, F5, E5, E5,
  D5, D5, E5, F5, E5, D5, C5
};

const uint8_t durations[] PROGMEM = {
  // verse
  8,8,8,8,4,4,
  8,8,8,8,8,8,8,8,
  8,8,8,8,4,4,
  8,8,8,8,8,8,8,8,
  8,8,8,8,4,4,
  8,8,8,8,8,8,8,8,
  8,8,8,8,4,4,
  8,8,8,8,8,8,8,8,
  // chorus
  4,8,8,8,8,8,4,
  8,8,8,8,8,8,8,
  4,8,8,8,8,8,4,
  8,8,8,8,8,8,8,
  4,8,8,8,8,8,4,
  8,8,8,8,8,8,4,
  4,8,8,8,8,4,
  8,8,8,8,8,8,8
};

#define NOTES (sizeof(melody) / sizeof(melody[0]))

// ---------- timer ----------
void tone_on(uint16_t freq) {
  if (freq == 0) return;

  uint16_t scaled = (freq * PITCH_NUM) / PITCH_DEN;
  uint16_t ocr = (F_CPU / (2UL * 8UL * scaled)) - 1;

  TCCR0A = (1 << WGM01);    // CTC
  TCCR0B = (1 << CS01);     // prescaler 8
  OCR0A  = ocr;
  TIMSK |= (1 << OCIE0A);
}

void tone_off(void) {
  TCCR0B = 0;
  TIMSK &= ~(1 << OCIE0A);
  PORTB &= ~(1 << BUZZER_PIN);
}

ISR(TIMER0_COMPA_vect) {
  PINB = (1 << BUZZER_PIN); // toggle PB0
}

// ---------- main ----------
int main(void) {
  DDRB |= (1 << BUZZER_PIN);
  sei();

  for (uint8_t i = 0; i < NOTES; i++) {

    uint16_t note = pgm_read_word(&melody[i]);
    uint8_t dur   = pgm_read_byte(&durations[i]);

    uint16_t duration = 1350 / dur;

    if (note != REST)
      tone_on(note);

    for (uint16_t t = 0; t < duration; t++)
      _delay_ms(1);

    tone_off();

    uint16_t gap = duration / 3;
    for (uint16_t t = 0; t < gap; t++)
      _delay_ms(1);
  }

  while (1); // play once
}
