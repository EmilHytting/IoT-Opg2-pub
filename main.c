#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

#include "Include/Timer/Timer.h"
#include "Include/UART/UART.h"

// Køen har en ubrugt plads, så læse- og skriveindekset kan vise, at den er tom.
#define UART1_QUEUE_SIZE 128U
#define UART1_QUEUE_MASK (UART1_QUEUE_SIZE - 1U)

static char uart1_queue[UART1_QUEUE_SIZE];
static uint8_t uart1_read;
static uint8_t uart1_write;

static void UART0_Init(void) {
  UBRR0 = MyUBRR;
  UCSR0A = 0;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

static void UART1_Init(void) {
  UBRR1 = MyUBRR;
  UCSR1A = 0;
  UCSR1B = (1 << TXEN1);
  UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}

static uint8_t UART1_FreeSpace(void) {
  return (uart1_read + UART1_QUEUE_SIZE - uart1_write - 1U) & UART1_QUEUE_MASK;
}

static void UART1_QueueChar(char character) {
  uart1_queue[uart1_write] = character;
  uart1_write = (uart1_write + 1U) & UART1_QUEUE_MASK;
}

static void ConvertLetterCase(char *character) {
  if (*character >= 'a' && *character <= 'z') {
    *character -= ('a' - 'A');
  } else if (*character >= 'A' && *character <= 'Z') {
    *character += ('a' - 'A');
  }
}

int main(void) {
  uint32_t seconds = 0;
  char message[40];

  DDRB |= (1 << GreenLedBit);

  UART0_Init();
  UART1_Init();
  Timer1_Init_ms(1000);

  sei();

  while (1) {
    if (uart1_read != uart1_write && (UCSR1A & (1 << UDRE1))) {
      UDR1 = uart1_queue[uart1_read];
      uart1_read = (uart1_read + 1U) & UART1_QUEUE_MASK;
    }

    if (timer_second_flag && UART1_FreeSpace() >= sizeof(message)) {
      uint8_t saved_sreg = SREG;
      cli();
      timer_second_flag = 0;
      SREG = saved_sreg;

      seconds++;
      snprintf(message, sizeof(message), "Sekunder efter opstart: %lu\r\n",
               (unsigned long)seconds);

      for (uint8_t i = 0; message[i] != '\0'; i++) {
        UART1_QueueChar(message[i]);
      }
    }

    // Behold plads til næste sekundlinje, mens UART0 sender og modtager.
    if (UART1_FreeSpace() > sizeof(message) &&
        (UCSR0A & (1 << RXC0)) && (UCSR0A & (1 << UDRE0))) {
      char character = UDR0;
      ConvertLetterCase(&character);
      UDR0 = character;
      UART1_QueueChar(character);
    }
  }
}
