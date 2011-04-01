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

#ifndef _TIMER_A_H
#define _TIMER_A_H

static inline void
timer_a_clock_source_taclk()
{
	TACTL &= ~(TASSEL1 | TASSEL0);
}

static inline void
timer_a_clock_source_aclk()
{
#ifdef ATOMIC
	TACTL = (TACTL & ~TASSEL1) | TASSEL0;
#else
	TACTL &= ~TASSEL1;
	TACTL |= TASSEL0;
#endif
}

static inline void
timer_a_clock_source_smclk()
{
#ifdef ATOMIC
	TACTL = (TACTL & ~TASSEL0) | TASSEL1;
#else
	TACTL |= TASSEL1;
	TACTL &= ~TASSEL0;
#endif
}

static inline void
timer_a_clock_source_inclk()
{
	TACTL |= TASSEL1 | TASSEL0;
}

static inline void
timer_a_clock_divide(unsigned int n)
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
timer_a_stop()
{
	TACTL &= ~(MC1 | MC0);
}

static inline void
timer_a_mode_up()
{
#ifdef ATOMIC
	TACTL = (TACTL & ~MC1) | MC0;
#else
	TACTL &= ~MC1;
	TACTL |= MC0;
#endif
}

static inline void
timer_a_mode_continuous()
{
#ifdef ATOMIC
	TACTL = (TACTL & ~MC0) | MC1;
#else
	TACTL |= MC1;
	TACTL &= ~MC0;
#endif
}

static inline void
timer_a_mode_updown()
{
	TACTL |= MC1 | MC0;
}

static inline void
timer_a_clear()             { TACTL |= TACLR; }
static inline void
timer_a_interrupt_enable()  { TACTL |= TAIE; }
static inline void
timer_a_interrupt_disable() { TACTL &= ~TAIE; }
static inline int
timer_a_interrupt_enabled() { return TACTL & TAIE; }
static inline void
timer_a_interrupt_raise()   { TACTL |= TAIFG; }
static inline void
timer_a_interrupt_clear()   { TACTL &= ~TAIFG; }
static inline int
timer_a_interrupt_flag()    { return TACTL & TAIFG; }
static inline unsigned int
timer_a_count()             { return TAR; }

#define define_capture_compare(n)\
	static inline void\
	timer_a_cc##n##_set(unsigned int v) { TACCR##n = v; }\
	static inline void\
	timer_a_cc##n##_add(unsigned int v) { TACCR##n += v; }\
	static inline unsigned int\
	timer_a_cc##n()                     { return TACCR##n; }\
	static inline void\
	timer_a_capture##n##_mode_none()    { TACCTL##n &= ~(CM1 | CM0); }\
	static inline void\
	timer_a_capture##n##_mode_rising()  { TACCTL##n &= ~CM1; TACCTL##n |= CM0; }\
	static inline void\
	timer_a_capture##n##_mode_falling() { TACCTL##n |= CM1; TACCTL##n &= ~CM0; }\
	static inline void\
	timoer_a_capture##n##_mode_both()   { TACCTL##n |= CM1 | CM0; }\
	static inline void\
	timer_a_cc##n##_mode_capture()      { TACCTL##n |= CAP; }\
	static inline void\
	timer_a_cc##n##_mode_compare()      { TACCTL##n &= ~CAP; }\
	static inline void\
	timer_a_cc##n##_interrupt_enable()  { TACCTL##n |= CCIE; }\
	static inline void\
	timer_a_cc##n##_interrupt_disable() { TACCTL##n &= ~CCIE; }\
	static inline int\
	timer_a_cc##n##_interrupt_enabled() { return TACCTL##n & CCIE; }\
	static inline void\
	timer_a_cc##n##_output_high()       { TACCTL##n |= OUT; }\
	static inline void\
	timer_a_cc##n##_output_low()       { TACCTL##n &= ~OUT; }\
	static inline int\
	timer_a_capture##n##_overflow()     { return TACCTL##n & COV; }\
	static inline void\
	timer_a_cc##n##_interrupt_raise()   { TACCTL##n |= CCIFG; }\
	static inline void\
	timer_a_cc##n##_interrupt_clear()   { TACCTL##n &= ~CCIFG; }\
	static inline void\
	timer_a_cc##n##_output_mode(unsigned int mode)\
	{ TACCTL##n |= mode << 5; }

define_capture_compare(0)
define_capture_compare(1)

#define timer_a_interrupt()     interrupt(TIMERA1_VECTOR) timer_a_interrupt()
#define timer_a_cc0_interrupt() interrupt(TIMERA0_VECTOR) timer_a_cc0_interrupt()
#define timer_a_cc1_interrupt() interrupt(TIMERA1_VECTOR) timer_a_cc1_interrupt()

#undef define_capture_compare
#endif
