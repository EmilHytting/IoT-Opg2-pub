#include "../../Include/Timer/Timer.h"
#include <avr/interrupt.h>
#include <avr/io.h>

volatile uint8_t timer_second_flag = 0;

void Timer1_Init_ms(uint16_t timeout_ms) {
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // CTC-mode med prescaler 256.

  uint32_t ticks = ((F_CPU / 256UL) * timeout_ms) / 1000UL;

  if (ticks > 0) {
    ticks--;
  }

  OCR1A = (uint16_t)ticks;
  TIMSK1 |= (1 << OCIE1A);
  TCCR1B = (1 << WGM12) | (1 << CS12);
}

ISR(TIMER1_COMPA_vect) {
  PORTB ^= (1 << GreenLedBit);
  timer_second_flag = 1;
}