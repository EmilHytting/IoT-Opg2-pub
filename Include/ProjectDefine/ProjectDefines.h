/*
 * ProjectDefines.h
 *
 * Created: 15-01-2018 14:56:37
 *  Author: ltpe
 */

#ifndef PROJECTDEFINES_H_
#define PROJECTDEFINES_H_

#define F_CPU 8000000UL
#define GreenLedBit PORTB1

#include <avr/io.h>
#include <avr/interrupt.h>

#if defined (_AVR_IOM168_H_) || defined (_AVR_IOM328P_H_)
#define USR0_Vect_Num USART_RX_vect

#endif

#if defined (_AVR_IOM2560_H_) || defined (_AVR_ATMEGA328PB_H_INCLUDED)
#define USR0_Vect_Num USART0_RX_vect 

#endif

#if defined(_AVR_IOM2560_H_) || defined (_AVR_ATMEGA328PB_H_INCLUDED)
#define YellowLedBit    PORTB2
#define RedLedBit		PORTB3
#endif

typedef void (*Function_Pointer_With_One_CharPointer_Parameter)(char *);

#endif /* PROJECTDEFINES_H_ */