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

#include <io.h>

#define define_pin_set(name, on, off) \
	static inline __attribute__((always_inline)) void\
	pin_##on(double pin)\
	{\
		int n = (int)(10*pin);\
		switch (n) {\
		case 10: case 11: case 12: case 13:\
		case 14: case 15: case 16: case 17:\
			P1##name |= 1 << (n % 10); break;\
		case 20: case 21: case 22: case 23:\
		case 24: case 25: case 26: case 27:\
			P2##name |= 1 << (n % 10); break;\
		}\
	}\
	static inline __attribute__((always_inline)) void\
	pin_##off(double pin)\
	{\
		int n = (int)(10*pin);\
		switch (n) {\
		case 10: case 11: case 12: case 13:\
		case 14: case 15: case 16: case 17:\
			P1##name &= ~(1 << (n % 10)); break;\
		case 20: case 21: case 22: case 23:\
		case 24: case 25: case 26: case 27:\
			P2##name &= ~(1 << (n % 10)); break;\
		}\
	}

#define define_pin_read(name, fname) \
	static inline __attribute__((always_inline)) int\
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

#define define_port(n)\
	static inline __attribute__((always_inline)) void\
	port##n##_direction_set(unsigned char v) { P##n##DIR = v; }\
	static inline __attribute__((always_inline)) void\
	port##n##_set(unsigned char v)           { P##n##OUT = v; }\
	static inline __attribute__((always_inline)) void\
	port##n##_setbits(unsigned char v)       { P##n##OUT &= v; }\
	static inline __attribute__((always_inline)) void\
	port##n##_clearbits(unsigned char v)     { P##n##OUT &= ~v; }\
	static inline __attribute__((always_inline)) unsigned char\
	port##n##_get()                          { return P##n##IN; }\
	static inline __attribute__((always_inline)) void\
	port##n##_interrupts_raise()             { P##n##IFG = 0xFF; }\
	static inline __attribute__((always_inline)) void\
	port##n##_interrupts_clear()             { P##n##IFG = 0x00; }

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
		P1SEL &= ~(1 << (n % 10));
#ifdef P1SEL2_
		P1SEL2 &= ~(1 << (n % 10));
#endif
		break;
	case 20: case 21: case 22: case 23:
	case 24: case 25: case 26: case 27:
		P2SEL &= ~(1 << (n % 10));
#ifdef P2SEL2_
		P2SEL2 &= ~(1 << (n % 10));
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
		P1SEL |= 1 << (n % 10);
#ifdef P1SEL2_
		P1SEL2 &= ~(1 << (n % 10));
#endif
		break;
	case 20: case 21: case 22: case 23:
	case 24: case 25: case 26: case 27:
		P2SEL |= 1 << (n % 10);
#ifdef P2SEL2_
		P2SEL2 &= ~(1 << (n % 10));
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
	case 10: case 11: case 12: case 13:
	case 14: case 15: case 16: case 17:
		P1SEL |= 1 << (n % 10);
#ifdef P1SEL2_
		P1SEL2 |= 1 << (n % 10);
#endif
		break;
	case 20: case 21: case 22: case 23:
	case 24: case 25: case 26: case 27:
		P2SEL |= 1 << (n % 10);
#ifdef P2SEL2_
		P2SEL2 |= 1 << (n % 10);
#endif
		break;
	}
}
#endif

define_port(1)
define_port(2)

#define port1_interrupt() interrupt(PORT1_VECTOR) port1_interrupt()
#define port2_interrupt() interrupt(PORT2_VECTOR) port2_interrupt()

#undef define_pin_set
#undef define_pin_read
#undef define_port
#endif
