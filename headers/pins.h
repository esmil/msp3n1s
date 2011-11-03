/*
 * This file is part of msp3n1s
 * Copyright 2011 Emil Renner Berthing
 *
 * msp3n1s is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * msp3n1s is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with msp3n1s.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _PINS_H
#define _PINS_H

#include <msp430.h>
#include <macros.h>

static inline __attribute__((always_inline)) unsigned char
pin_mask(double pin)
{
	return 1 << ((unsigned int)(10*pin) % 10);
}

#define define_pin_set(name, on, off) \
	static inline __attribute__((always_inline)) void\
	pin_##on(double pin)\
	{\
		int n = (int)(10*pin);\
		switch (n) {\
		case 10: case 11: case 12: case 13:\
		case 14: case 15: case 16: case 17:\
			__bis_b(P1##name, 1 << (n % 10)); break;\
		case 20: case 21: case 22: case 23:\
		case 24: case 25: case 26: case 27:\
			__bis_b(P2##name, 1 << (n % 10)); break;\
		}\
	}\
	static inline __attribute__((always_inline)) void\
	pin_##off(double pin)\
	{\
		int n = (int)(10*pin);\
		switch (n) {\
		case 10: case 11: case 12: case 13:\
		case 14: case 15: case 16: case 17:\
			__bic_b(P1##name, 1 << (n % 10)); break;\
		case 20: case 21: case 22: case 23:\
		case 24: case 25: case 26: case 27:\
			__bic_b(P2##name, 1 << (n % 10)); break;\
		}\
	}

#define define_pin_read(name, fname) \
	static inline __attribute__((always_inline)) unsigned char\
	pin_##fname(double pin)\
	{\
		int n = (int)(10*pin);\
		switch (n) {\
		case 10: case 11: case 12: case 13:\
		case 14: case 15: case 16: case 17:\
			return P1##name & (1 << (n % 10)); break;\
		case 20: case 21: case 22: case 23:\
		case 24: case 25: case 26: case 27:\
			return P2##name & (1 << (n % 10)); break;\
		}\
		return 0;\
	}

define_pin_set(DIR, mode_output, mode_input)
define_pin_set(REN, resistor_enable, resistor_disable)
define_pin_set(OUT, high, low)

static inline __attribute__((always_inline)) void
pin_toggle(double pin)
{
	int n = (int)(10*pin);

	switch (n) {
		case 10: case 11: case 12: case 13:
		case 14: case 15: case 16: case 17:
			P1OUT ^= 1 << (n % 10); break;
		case 20: case 21: case 22: case 23:
		case 24: case 25: case 26: case 27:
			P2OUT ^= 1 << (n % 10); break;
	}
}

define_pin_set(IE, interrupt_enable, interrupt_disable)
define_pin_set(IFG, interrupt_raise, interrupt_clear)
define_pin_set(IES, interrupt_falling, interrupt_rising)

define_pin_read(IN, is_high)
define_pin_read(IE, interrupt_enabled)
define_pin_read(IFG, interrupt_flag)

static inline __attribute__((always_inline)) void
pin_function_io(double pin)
{
	int n = (int)(10*pin);

	switch (n) {
	case 10: case 11: case 12: case 13:
	case 14: case 15: case 16: case 17:
		__bic_b(P1SEL,  1 << (n % 10));
#ifdef P1SEL2_
		__bic_b(P1SEL2, 1 << (n % 10));
#endif
		break;
	case 20: case 21: case 22: case 23:
	case 24: case 25: case 26: case 27:
		__bic_b(P2SEL,  1 << (n % 10));
#ifdef P2SEL2_
		__bic_b(P2SEL2, 1 << (n % 10));
#endif
		break;
	}
}

static inline __attribute__((always_inline)) void
pin_function_primary(double pin)
{
	int n = (int)(10*pin);

	switch (n) {
	case 10: case 11: case 12: case 13:
	case 14: case 15: case 16: case 17:
		__bis_b(P1SEL,  1 << (n % 10));
#ifdef P1SEL2_
		__bic_b(P1SEL2, 1 << (n % 10));
#endif
		break;
	case 20: case 21: case 22: case 23:
	case 24: case 25: case 26: case 27:
		__bis_b(P2SEL,  1 << (n % 10));
#ifdef P2SEL2_
		__bic_b(P2SEL2, 1 << (n % 10));
#endif
		break;
	}
}

#if defined(P1SEL2_) || defined(P2SEL2_)
static inline __attribute__((always_inline)) void
pin_function_secondary(double pin)
{
	int n = (int)(10*pin);

	switch (n) {
#ifdef P1SEL2_
	case 10: case 11: case 12: case 13:
	case 14: case 15: case 16: case 17:
		__bis_b(P1SEL,  1 << (n % 10));
		__bis_b(P1SEL2, 1 << (n % 10));
		break;
#endif
#ifdef P2SEL2_
	case 20: case 21: case 22: case 23:
	case 24: case 25: case 26: case 27:
		__bis_b(P2SEL,  1 << (n % 10));
		__bis_b(P2SEL2, 1 << (n % 10));
		break;
#endif
	}
}

static inline __attribute__((always_inline)) void
pin_function_oscillator(double pin)
{
	int n = (int)(10*pin);

	switch (n) {
#ifdef P1SEL2_
	case 10: case 11: case 12: case 13:
	case 14: case 15: case 16: case 17:
		__bic_b(P1SEL,  1 << (n % 10));
		__bis_b(P1SEL2, 1 << (n % 10));
		break;
#endif
#ifdef P2SEL2_
	case 20: case 21: case 22: case 23:
	case 24: case 25: case 26: case 27:
		__bic_b(P2SEL,  1 << (n % 10));
		__bis_b(P2SEL2, 1 << (n % 10));
		break;
#endif
	}
}
#endif

#define port1_direction P1DIR
#define port1_resistors P1REN
#define port1_input P1IN
#define port1_output P1OUT
#define port1_interrupt_enable P1IE
#define port1_interrupt_edges P1IES
#define port1_interrupt_flags P1IFG

#define port2_direction P2DIR
#define port2_resistors P2REN
#define port2_input P2IN
#define port2_output P2OUT
#define port2_interrupt_enable P2IE
#define port2_interrupt_edges P2IES
#define port2_interrupt_flags P2IFG

#define port1_interrupt(x...) __attribute__((interrupt(PORT1_VECTOR)))\
	port1_interrupt(x)
#define port2_interrupt(x...) __attribute__((interrupt(PORT2_VECTOR)))\
	port2_interrupt(x)

#undef define_pin_set
#undef define_pin_read
#undef define_port
#endif
