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

#include <msp430.h>
#include <macros.h>

static inline void
timera_clock_source_taclk(void)
{
	__bic_w(TASSEL1 | TASSEL0, TACTL);
}

static inline void
timera_clock_source_aclk(void)
{
#ifdef ATOMIC
	TACTL = (TACTL & ~TASSEL1) | TASSEL0;
#else
	__bic_w(TASSEL1, TACTL);
	__bis_w(TASSEL0, TACTL);
#endif
}

static inline void
timera_clock_source_smclk(void)
{
#ifdef ATOMIC
	TACTL = (TACTL & ~TASSEL0) | TASSEL1;
#else
	__bis_w(TASSEL1, TACTL);
	__bic_w(TASSEL0, TACTL);
#endif
}

static inline void
timera_clock_source_inclk(void)
{
	__bis_w(TASSEL1 | TASSEL0, TACTL);
}

static inline void
timera_clock_divide(unsigned int n)
{
	switch (n) {
	case 1:
		__bic_w(ID1 | ID0, TACTL);
		break;
	case 2:
#ifdef ATOMIC
		TACTL = (TACTL & ~ID1) | ID0;
#else
		__bic_w(ID1, TACTL);
		__bis_w(ID0, TACTL);
#endif
		break;
	case 4:
#ifdef ATOMIC
		TACTL = (TACTL & ~ID0) | ID1;
#else
		__bis_w(ID1, TACTL);
		__bic_w(ID0, TACTL);
#endif
		break;
	case 8:
		__bis_w(ID1 | ID0, TACTL);
		break;
	}
}

static inline void
timera_off(void)
{
	__bic_w(MC1 | MC0, TACTL);
}

static inline void
timera_mode_up(void)
{
#ifdef ATOMIC
	TACTL = (TACTL & ~MC1) | MC0;
#else
	__bic_w(MC1, TACTL);
	__bis_w(MC0, TACTL);
#endif
}

static inline void
timera_mode_continuous(void)
{
#ifdef ATOMIC
	TACTL = (TACTL & ~MC0) | MC1;
#else
	__bis_w(MC1, TACTL);
	__bic_w(MC0, TACTL);
#endif
}

static inline void
timera_mode_updown(void)
{
	__bis_w(MC1 | MC0, TACTL);
}

static inline void
timera_clear(void)             { __bis_w(TACLR, TACTL); }
static inline void
timera_interrupt_enable(void)  { __bis_w(TAIE, TACTL); }
static inline void
timera_interrupt_disable(void) { __bic_w(TAIE, TACTL); }
static inline unsigned int
timera_interrupt_enabled(void) { return TACTL & TAIE; }
static inline void
timera_interrupt_raise(void)   { __bis_w(TAIFG, TACTL); }
static inline void
timera_interrupt_clear(void)   { __bic_w(TAIFG, TACTL); }
static inline unsigned int
timera_interrupt_flag(void)    { return TACTL & TAIFG; }
static inline unsigned int
timera_count(void)             { return TAR; }

#define define_capture_compare(n)\
	static inline void\
	timera_cc##n##_set(unsigned int v)     { TACCR##n = v; }\
	static inline void\
	timera_cc##n##_add(unsigned int v)     { TACCR##n += v; }\
	static inline unsigned int\
	timera_cc##n(void)                     { return TACCR##n; }\
	static inline void\
	timera_cc##n##_capture_none(void)      { __bic_w(CM1 | CM0, TACCTL##n); }\
	static inline void\
	timera_cc##n##_capture_rising(void)    { __bic_w(CM1, TACCTL##n); __bis_w(CM0, TACCTL##n); }\
	static inline void\
	timera_cc##n##_capture_falling(void)   { __bis_w(CM1, TACCTL##n); __bic_w(CM0, TACCTL##n); }\
	static inline void\
	timera_cc##n##_capture_both(void)      { __bis_w(CM1 | CM0, TACCTL##n); }\
	static inline void\
	timera_cc##n##_input_a(void)           { __bic_w(CCIS1 | CCIS0, TACCTL##n); }\
	static inline void\
	timera_cc##n##_input_b(void)           { __bic_w(CCIS1, TACCTL##n); __bis_w(CCIS0, TACCTL##n); }\
	static inline void\
	timera_cc##n##_input_gnd(void)         { __bis_w(CCIS1, TACCTL##n); __bic_w(CCIS0, TACCTL##n); }\
	static inline void\
	timera_cc##n##_input_vcc(void)         { __bis_w(CCIS1 | CCIS0, TACCTL##n); }\
	static inline void\
	timera_cc##n##_capture(void)           { TACCTL##n ^= CCIS0; }\
	static inline void\
	timera_cc##n##_mode_capture(void)      { __bis_w(CAP, TACCTL##n); }\
	static inline void\
	timera_cc##n##_mode_compare(void)      { __bic_w(CAP, TACCTL##n); }\
	static inline void\
	timera_cc##n##_interrupt_enable(void)  { __bis_w(CCIE, TACCTL##n); }\
	static inline void\
	timera_cc##n##_interrupt_disable(void) { __bic_w(CCIE, TACCTL##n); }\
	static inline unsigned int\
	timera_cc##n##_interrupt_enabled(void) { return TACCTL##n & CCIE; }\
	static inline void\
	timera_cc##n##_output_high(void)       { __bis_w(OUT, TACCTL##n); }\
	static inline void\
	timera_cc##n##_output_low(void)        { __bic_w(OUT, TACCTL##n); }\
	static inline unsigned int\
	timera_cc##n##_capture_overflow(void)  { return TACCTL##n & COV; }\
	static inline void\
	timera_cc##n##_interrupt_raise(void)   { __bis_w(CCIFG, TACCTL##n); }\
	static inline void\
	timera_cc##n##_interrupt_clear(void)   { __bic_w(CCIFG, TACCTL##n); }\
	static inline void\
	timera_cc##n##_output_mode(unsigned int mode)\
	{ __bis_w(mode << 5, TACCTL##n); }

define_capture_compare(0)
define_capture_compare(1)

#define timera_interrupt(x...) __attribute__((interrupt(TIMERA1_VECTOR)))\
	timera_interrupt(x)
#define timera_cc0_interrupt(x...) __attribute__((interrupt(TIMERA0_VECTOR)))\
	timera_cc0_interrupt(x)
#define timera_cc1_interrupt(x...) __attribute__((interrupt(TIMERA1_VECTOR)))\
	timera_cc1_interrupt(x)

#undef define_capture_compare
#endif
