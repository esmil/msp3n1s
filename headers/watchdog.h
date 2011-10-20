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

#ifndef _WATCHDOG_H
#define _WATCHDOG_H

#include <msp430.h>
#include <macros.h>

static inline void
watchdog_timer_hold(void)         { WDTCTL |= WDTHOLD; }
static inline void
watchdog_timer_resume(void)       { WDTCTL &= ~WDTHOLD; }
static inline void
watchdog_nmi_edge_rising(void)    { WDTCTL |= WDTNMIES; }
static inline void
watchdog_nmi_edge_falling(void)   { WDTCTL &= ~WDTNMIES; }
static inline void
watchdog_nmi(void)                { WDTCTL |= WDTNMI; }
static inline void
watchdog_reset(void)              { WDTCTL &= ~WDTNMI; }
static inline void
watchdog_mode_watchdog(void)      { WDTCTL |= WDTTMSEL; }
static inline void
watchdog_mode_timer(void)         { WDTCTL &= ~WDTTMSEL; }
static inline void
watchdog_counter_clear(void)      { WDTCTL |= WDTCNTCL; }
static inline void
watchdog_clock_source_aclk(void)  { WDTCTL |= WDTTMSEL; }
static inline void
watchdog_clock_source_smclk(void) { WDTCTL &= ~WDTTMSEL; }

static inline void
watchdog_timer_interval(unsigned int n)
{
	switch (n) {
	case 32768:
		WDTCTL &= ~(WDTIS1 | WDTIS0);
		break;

	case 8192:
#ifdef ATOMIC
		WDTCTL = (WDTCTL & ~WDTIS1) | WDTIS0;
#else
		WDTCTL &= ~WDTIS1;
		WDTCTL |= WDTIS0;
#endif
		break;

	case 512:
#ifdef ATOMIC
		WDTCTL = (WDTCTL & ~WDTIS0) | WDTIS1;
#else
		WDTCTL |= WDTIS1;
		WDTCTL &= ~WDTIS0;
#endif
		break;

	case 8:
		WDTCTL |= WDTIS1 | WDTIS0;
		break;
	}
}

static inline void
watchdog_timer_interrupt_enable(void)  { __bis_b(IE1, WDTIE); }
static inline void
watchdog_timer_interrupt_disable(void) { __bic_b(IE1, WDTIE); }
static inline unsigned char
watchdog_timer_interrupt_enabled(void) { return IE1 & WDTIE; }
static inline void
watchdog_nmi_interrupt_enable(void)    { __bis_b(IE1, NMIIE); }
static inline void
watchdog_nmi_interrupt_disable(void)   { __bic_b(IE1, NMIIE); }
static inline unsigned char
watchdog_nmi_interrupt_enabled(void)   { return IE1 & NMIIE; }

static inline void
watchdog_timer_interrupt_raise(void)   { __bis_b(IFG1, WDTIFG); }
static inline void
watchdog_timer_interrupt_clear(void)   { __bic_b(IFG1, WDTIFG); }
static inline unsigned char
watchdog_timer_interrupt_flag(void)    { return IFG1 & WDTIFG; }
static inline void
watchdog_nmi_interrupt_raise(void)     { __bis_b(IFG1, NMIIFG); }
static inline void
watchdog_nmi_interrupt_clear(void)     { __bic_b(IFG1, NMIIFG); }
static inline unsigned char
watchdog_nmi_interrupt_flag(void)      { return IFG1 & NMIIFG; }

static inline void
watchdog_off(void)          { WDTCTL = WDTPW | WDTHOLD; }

static inline void
watchdog_timer_32ms(void)   { WDTCTL = WDT_MDLY_32; }
static inline void
watchdog_timer_8ms(void)    { WDTCTL = WDT_MDLY_8; }
static inline void
watchdog_timer_0_5ms(void)  { WDTCTL = WDT_MDLY_0_5; }
static inline void
watchdog_timer_0_064ms(void)  { WDTCTL = WDT_MDLY_0_064; }

static inline void
watchdog_timer_1000ms(void) { WDTCTL = WDT_ADLY_1000; }
static inline void
watchdog_timer_250ms(void)  { WDTCTL = WDT_ADLY_250; }
static inline void
watchdog_timer_16ms(void)   { WDTCTL = WDT_ADLY_16; }
static inline void
watchdog_timer_1_9ms(void)  { WDTCTL = WDT_ADLY_1_9; }

static inline void
watchdog_mode_32ms(void)    { WDTCTL = WDT_MRST_32; }
static inline void
watchdog_mode_8ms(void)     { WDTCTL = WDT_MRST_8; }
static inline void
watchdog_mode_0_5ms(void)   { WDTCTL = WDT_MRST_0_5; }
static inline void
watchdog_mode_0_064ms(void) { WDTCTL = WDT_MRST_0_064; }

#define watchdog_interrupt(x...) __attribute__((interrupt(WDT_VECTOR)))\
	watchdog_interrupt(x)

#endif
