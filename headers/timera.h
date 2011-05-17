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

#ifndef _TIMERA_H
#define _TIMERA_H

#include <io.h>

static inline void
timera_clock_source_taclk()
{
	TACTL &= ~(TASSEL1 | TASSEL0);
}

static inline void
timera_clock_source_aclk()
{
#ifdef ATOMIC
	TACTL = (TACTL & ~TASSEL1) | TASSEL0;
#else
	TACTL &= ~TASSEL1;
	TACTL |= TASSEL0;
#endif
}

static inline void
timera_clock_source_smclk()
{
#ifdef ATOMIC
	TACTL = (TACTL & ~TASSEL0) | TASSEL1;
#else
	TACTL |= TASSEL1;
	TACTL &= ~TASSEL0;
#endif
}

static inline void
timera_clock_source_inclk()
{
	TACTL |= TASSEL1 | TASSEL0;
}

static inline void
timera_clock_divide(unsigned int n)
{
	switch (n) {
	case 1:
		TACTL &= ~(ID1 | ID0);
		break;
	case 2:
#ifdef ATOMIC
		TACTL = (TACTL & ~ID1) | ID0;
#else
		TACTL &= ~ID1;
		TACTL |= ID0;
#endif
		break;
	case 4:
#ifdef ATOMIC
		TACTL = (TACTL & ~ID0) | ID1;
#else
		TACTL |= ID1;
		TACTL &= ~ID0;
#endif
		break;
	case 8:
		TACTL |= ID1 | ID0;
		break;
	}
}

static inline void
timera_off()
{
	TACTL &= ~(MC1 | MC0);
}

static inline void
timera_mode_up()
{
#ifdef ATOMIC
	TACTL = (TACTL & ~MC1) | MC0;
#else
	TACTL &= ~MC1;
	TACTL |= MC0;
#endif
}

static inline void
timera_mode_continuous()
{
#ifdef ATOMIC
	TACTL = (TACTL & ~MC0) | MC1;
#else
	TACTL |= MC1;
	TACTL &= ~MC0;
#endif
}

static inline void
timera_mode_updown()
{
	TACTL |= MC1 | MC0;
}

static inline void
timera_clear()             { TACTL |= TACLR; }
static inline void
timera_interrupt_enable()  { TACTL |= TAIE; }
static inline void
timera_interrupt_disable() { TACTL &= ~TAIE; }
static inline int
timera_interrupt_enabled() { return TACTL & TAIE; }
static inline void
timera_interrupt_raise()   { TACTL |= TAIFG; }
static inline void
timera_interrupt_clear()   { TACTL &= ~TAIFG; }
static inline int
timera_interrupt_flag()    { return TACTL & TAIFG; }
static inline unsigned int
timera_count()             { return TAR; }

#define define_capture_compare(n)\
	static inline void\
	timera_cc##n##_set(unsigned int v) { TACCR##n = v; }\
	static inline void\
	timera_cc##n##_add(unsigned int v) { TACCR##n += v; }\
	static inline unsigned int\
	timera_cc##n()                     { return TACCR##n; }\
	static inline void\
	timera_cc##n##_capture_none()      { TACCTL##n &= ~(CM1 | CM0); }\
	static inline void\
	timera_cc##n##_capture_rising()    { TACCTL##n &= ~CM1; TACCTL##n |= CM0; }\
	static inline void\
	timera_cc##n##_capture_falling()   { TACCTL##n |= CM1; TACCTL##n &= ~CM0; }\
	static inline void\
	timera_cc##n##_capture_both()      { TACCTL##n |= CM1 | CM0; }\
	static inline void\
	timera_cc##n##_input_a()        { TACCTL##n &= ~(CCIS1 | CCIS0); }\
	static inline void\
	timera_cc##n##_input_b()        { TACCTL##n &= ~CCIS1; TACCTL##n |= CCIS0; }\
	static inline void\
	timera_cc##n##_input_gnd()      { TACCTL##n |= CCIS1; TACCTL##n &= ~CCIS0; }\
	static inline void\
	timera_cc##n##_input_vcc()      { TACCTL##n |= CCIS1 | CCIS0; }\
	static inline void\
	timera_cc##n##_capture()        { TACCTL##n ^= CCIS0; }\
	static inline void\
	timera_cc##n##_mode_capture()      { TACCTL##n |= CAP; }\
	static inline void\
	timera_cc##n##_mode_compare()      { TACCTL##n &= ~CAP; }\
	static inline void\
	timera_cc##n##_interrupt_enable()  { TACCTL##n |= CCIE; }\
	static inline void\
	timera_cc##n##_interrupt_disable() { TACCTL##n &= ~CCIE; }\
	static inline int\
	timera_cc##n##_interrupt_enabled() { return TACCTL##n & CCIE; }\
	static inline void\
	timera_cc##n##_output_high()       { TACCTL##n |= OUT; }\
	static inline void\
	timera_cc##n##_output_low()        { TACCTL##n &= ~OUT; }\
	static inline int\
	timera_cc##n##_capture_overflow()  { return TACCTL##n & COV; }\
	static inline void\
	timera_cc##n##_interrupt_raise()   { TACCTL##n |= CCIFG; }\
	static inline void\
	timera_cc##n##_interrupt_clear()   { TACCTL##n &= ~CCIFG; }\
	static inline void\
	timera_cc##n##_output_mode(unsigned int mode)\
	{ TACCTL##n |= mode << 5; }

define_capture_compare(0)
define_capture_compare(1)

#define timera_interrupt()     interrupt(TIMERA1_VECTOR) timera_interrupt()
#define timera_cc0_interrupt() interrupt(TIMERA0_VECTOR) timera_cc0_interrupt()
#define timera_cc1_interrupt() interrupt(TIMERA1_VECTOR) timera_cc1_interrupt()

#undef define_capture_compare
#endif
