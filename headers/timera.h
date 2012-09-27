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

#define TIMERA_CLOCK_TACLK        (0x0000)
#define TIMERA_CLOCK_ACLK         (0x0100)
#define TIMERA_CLOCK_SMCLK        (0x0200)
#define TIMERA_CLOCK_INCLK        (0x0300)
#define TIMERA_CLOCK_DIV1         (0x0000)
#define TIMERA_CLOCK_DIV2         (0x0040)
#define TIMERA_CLOCK_DIV4         (0x0080)
#define TIMERA_CLOCK_DIV8         (0x00C0)
#define TIMERA_MODE_STOP          (0x0000)
#define TIMERA_MODE_UP            (0x0010)
#define TIMERA_MODE_CONTINUOUS    (0x0020)
#define TIMERA_MODE_UPDOWN        (0x0030)
#define TIMERA_CLEAR              (0x0004)
#define TIMERA_INTERRUPT_DISABLE  (0x0000)
#define TIMERA_INTERRUPT_ENABLE   (0x0002)
#define TIMERA_INTERRUPT_FLAG     (0x0001)

#define TIMERA_CC_CAPTURE_NONE       (0x0100)
#define TIMERA_CC_CAPTURE_RISING     (0x4100)
#define TIMERA_CC_CAPTURE_FALLING    (0x8100)
#define TIMERA_CC_CAPTURE_BOTH       (0xC100)
#define TIMERA_CC_INPUT_A            (0x0000)
#define TIMERA_CC_INPUT_B            (0x1000)
#define TIMERA_CC_INPUT_GND          (0x2000)
#define TIMERA_CC_INPUT_VCC          (0x3000)
#define TIMERA_CC_CAPTURE_ASYNC      (0x0000)
#define TIMERA_CC_CAPTURE_SYNC       (0x0800)
#define TIMERA_CC_OUTPUT_BIT         (0x0000)
#define TIMERA_CC_OUTPUT_SET         (0x0020)
#define TIMERA_CC_OUTPUT_TOGGLERESET (0x0040)
#define TIMERA_CC_OUTPUT_SETRESET    (0x0060)
#define TIMERA_CC_OUTPUT_TOGGLE      (0x0080)
#define TIMERA_CC_OUTPUT_RESET       (0x00A0)
#define TIMERA_CC_OUTPUT_TOGGLESET   (0x00C0)
#define TIMERA_CC_OUTPUT_RESETSET    (0x00E0)
#define TIMERA_CC_INTERRUPT_ENABLE   (0x0010)
#define TIMERA_CC_INTERRUPT_DISABLE  (0x0000)
#define TIMERA_CC_OUTPUT_HIGH        (0x0004)
#define TIMERA_CC_OUTPUT_LOW         (0x0000)
#define TIMERA_CC_INTERRUPT_FLAG     (0x0001)

#define define_control(prefix, reg)\
	static inline void\
	prefix##_config(unsigned int c)   { reg = c; }\
	static inline void\
	prefix##_off(void)                { reg = 0; }\
	static inline void\
	prefix##_clock_source_taclk(void) { __bic_w(reg, TASSEL1 | TASSEL0); }\
	static inline void\
	prefix##_clock_source_aclk(void)  { __bic_w(reg, TASSEL1); __bis_w(reg, TASSEL0); }\
	static inline void\
	prefix##_clock_source_smclk(void) { __bic_w(reg, TASSEL0); __bis_w(reg, TASSEL1); }\
	static inline void\
	prefix##_clock_source_inclk(void) { __bis_w(reg, TASSEL1 | TASSEL0); }\
	static inline void\
	prefix##_clock_divide(unsigned int n)\
	{\
		switch (n) {\
		case 1:\
			__bic_w(reg, ID1 | ID0);\
			break;\
		case 2:\
			__bic_w(reg, ID1);\
			__bis_w(reg, ID0);\
			break;\
		case 4:\
			__bis_w(reg, ID1);\
			__bic_w(reg, ID0);\
			break;\
		case 8:\
			__bis_w(reg, ID1 | ID0);\
			break;\
		}\
	}\
	static inline void\
	prefix##_stop(void)              { __bic_w(reg, MC1 | MC0); }\
	static inline void\
	prefix##_mode_up(void)           { __bic_w(reg, MC1); __bis_w(reg, MC0); }\
	static inline void\
	prefix##_mode_continuous(void)   { __bis_w(reg, MC1); __bic_w(reg, MC0); }\
	static inline void\
	prefix##_mode_updown(void)       {  __bis_w(reg, MC1 | MC0); }\
	static inline void\
	prefix##_clear(void)             { __bis_w(reg, TACLR); }\
	static inline void\
	prefix##_interrupt_enable(void)  { __bis_w(reg, TAIE); }\
	static inline void\
	prefix##_interrupt_disable(void) { __bic_w(reg, TAIE); }\
	static inline unsigned int\
	prefix##_interrupt_enabled(void) { return reg & TAIE; }\
	static inline void\
	prefix##_interrupt_raise(void)   { __bis_w(reg, TAIFG); }\
	static inline void\
	prefix##_interrupt_clear(void)   { __bic_w(reg, TAIFG); }\
	static inline unsigned int\
	prefix##_interrupt_flag(void)    { return reg & TAIFG; }

#define define_capture_compare(prefix, reg)\
	static inline void\
	prefix##_config(unsigned int c)  { reg = c; }\
	static inline void\
	prefix##_off(void)               { reg = 0; }\
	static inline void\
	prefix##_capture_none(void)      { __bic_w(reg, CM1 | CM0); }\
	static inline void\
	prefix##_capture_rising(void)    { __bic_w(reg, CM1); __bis_w(reg, CM0); }\
	static inline void\
	prefix##_capture_falling(void)   { __bis_w(reg, CM1); __bic_w(reg, CM0); }\
	static inline void\
	prefix##_capture_both(void)      { __bis_w(reg, CM1 | CM0); }\
	static inline void\
	prefix##_input_a(void)           { __bic_w(reg, CCIS1 | CCIS0); }\
	static inline void\
	prefix##_input_b(void)           { __bic_w(reg, CCIS1); __bis_w(reg, CCIS0); }\
	static inline void\
	prefix##_input_gnd(void)         { __bis_w(reg, CCIS1); __bic_w(reg, CCIS0); }\
	static inline void\
	prefix##_input_vcc(void)         { __bis_w(reg, CCIS1 | CCIS0); }\
	static inline void\
	prefix##_capture(void)           { reg ^= CCIS0; }\
	static inline void\
	prefix##_capture_sync(void)      { __bis_w(reg, SCS); }\
	static inline void\
	prefix##_capture_async(void)     { __bic_w(reg, SCS); }\
	static inline unsigned int\
	prefix##_input_sync(void)        { return reg & SCCI; }\
	static inline void\
	prefix##_mode_capture(void)      { __bis_w(reg, CAP); }\
	static inline void\
	prefix##_mode_compare(void)      { __bic_w(reg, CAP); }\
	static inline unsigned int\
	prefix##_mode_is_capture(void)   { return reg & CAP; }\
	static inline void\
	prefix##_interrupt_enable(void)  { __bis_w(reg, CCIE); }\
	static inline void\
	prefix##_interrupt_disable(void) { __bic_w(reg, CCIE); }\
	static inline unsigned int\
	prefix##_interrupt_enabled(void) { return reg & CCIE; }\
	static inline unsigned int\
	prefix##_input_async(void)       { return reg & CCI; }\
	static inline void\
	prefix##_output_high(void)       { __bis_w(reg, OUT); }\
	static inline void\
	prefix##_output_low(void)        { __bic_w(reg, OUT); }\
	static inline void\
	prefix##_overflow_clear(void)    { __bic_w(reg, COV); }\
	static inline unsigned int\
	prefix##_overflow(void)          { return reg & COV; }\
	static inline void\
	prefix##_interrupt_raise(void)   { __bis_w(reg, CCIFG); }\
	static inline void\
	prefix##_interrupt_clear(void)   { __bic_w(reg, CCIFG); }\
	static inline unsigned int\
	prefix##_interrupt_flag(void)    { return reg & CCIFG; }\
	static inline void\
	prefix##_output_mode(unsigned int mode)\
	{ __bis_w(reg, mode << 5); }

#if defined(__MSP430_HAS_TA3__)

define_control(timera, TA0CTL)
define_capture_compare(timera_cc0, TA0CCTL0)
define_capture_compare(timera_cc1, TA0CCTL1)
define_capture_compare(timera_cc2, TA0CCTL2)

#define timera_count TA0R
#define timera_cc0 TA0CCR0
#define timera_cc1 TA0CCR1
#define timera_cc2 TA0CCR2

#define timera_interrupt(x...) __attribute__((interrupt(TIMER0_A1_VECTOR)))\
	timera_interrupt(x)
#define timera_cc0_interrupt(x...) __attribute__((interrupt(TIMER0_A0_VECTOR)))\
	timera_cc0_interrupt(x)

#if defined(__MSP430_HAS_T1A3__)

define_control(timera1, TA1CTL)
define_capture_compare(timera1_cc0, TA1CCTL0)
define_capture_compare(timera1_cc1, TA1CCTL1)
define_capture_compare(timera1_cc2, TA1CCTL2)

#define timera1_count TA1R
#define timera1_cc0 TA1CCR0
#define timera1_cc1 TA1CCR1
#define timera1_cc2 TA1CCR2

#define timera1_interrupt(x...) __attribute__((interrupt(TIMER1_A1_VECTOR)))\
	timera1_interrupt(x)
#define timera1_cc0_interrupt(x...) __attribute__((interrupt(TIMER1_A0_VECTOR)))\
	timera1_cc0_interrupt(x)
#endif

#elif defined(__MSP430_HAS_TA2__)

define_control(timera, TACTL)
define_capture_compare(timera_cc0, TACCTL0)
define_capture_compare(timera_cc1, TACCTL1)

#define timera_count TAR
#define timera_cc0 TACCR0
#define timera_cc1 TACCR1

#define timera_interrupt(x...) __attribute__((interrupt(TIMERA1_VECTOR)))\
	timera_interrupt(x)
#define timera_cc0_interrupt(x...) __attribute__((interrupt(TIMERA0_VECTOR)))\
	timera_cc0_interrupt(x)

#else
#error "Unknown Timer_A module for this chip"
#endif

#undef define_control
#undef define_capture_compare

#endif
