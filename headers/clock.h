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

#ifndef _CLOCK_H
#define _CLOCK_H

#include <msp430.h>
#include <macros.h>

static inline void
clock_init_1MHz(void)
{
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL  = CALDCO_1MHZ;
}

static inline void
clock_mclk_1MHz(void)
{
	BCSCTL1 = (BCSCTL1 & ~(RSEL3 | RSEL2 | RSEL1 | RSEL0)) | CALBC1_1MHZ;
	DCOCTL  = CALDCO_1MHZ;
}

static inline void
clock_xt2_off(void)              { __bis_b(XT2OFF, BCSCTL1); }
static inline void
clock_xt2_on(void)               { __bic_b(XT2OFF, BCSCTL1); }
static inline void
clock_lfxt1_mode_high_freq(void) { __bis_b(XTS, BCSCTL1); }
static inline void
clock_lfxt1_mode_low_freq(void)  { __bic_b(XTS, BCSCTL1); }

static inline void
clock_aclk_divide(unsigned int n)
{
	switch (n) {
	case 1:
		__bic_b(DIVA1 | DIVA0, BCSCTL1);
		break;

	case 2:
#ifdef ATOMIC
		BCSCTL1 = (BCSCTL1 & ~DIVA1) | DIVA0;
#else
		__bic_b(DIVA1, BCSCTL1);
		__bis_b(DIVA0, BCSCTL1);
#endif
		break;

	case 4:
#ifdef ATOMIC
		BCSCTL1 = (BCSCTL1 & ~DIVA0) | DIVA1;
#else
		__bis_b(DIVA1, BCSCTL1);
		__bic_b(DIVA0, BCSCTL1);
#endif
		break;

	case 8:
		__bis_b(DIVA1 | DIVA0, BCSCTL1);
		break;
	}
}

static inline void
clock_mclk_source_dco(void)   { __bic_b(SELM1 | SELM0, BCSCTL2); }
static inline void
clock_mclk_source_xt2(void)   { BCSCTL2 = (BCSCTL2 & ~SELM0) | SELM0; }
static inline void
clock_mclk_source_lfxt1(void) { __bis_b(SELM1 | SELM0, BCSCTL2); }
static inline void
clock_mclk_source_vlo(void)   { __bis_b(SELM1 | SELM0, BCSCTL2); }

static inline void
clock_mclk_divide(unsigned int n)
{
	switch (n) {
	case 1:
		__bic_b(DIVM1 | DIVM0, BCSCTL2);
		break;

	case 2:
#ifdef ATOMIC
		BCSCTL2 = (BCSCTL2 & ~DIVM1) | DIVM0;
#else
		__bic_b(DIVM1, BCSCTL2);
		__bis_b(DIVM0, BCSCTL2);
#endif
		break;

	case 4:
#ifdef ATOMIC
		BCSCTL2 = (BCSCTL2 & ~DIVM0) | DIVM1;
#else
		__bis_b(DIVM1, BCSCTL2);
		__bic_b(DIVM0, BCSCTL2);
#endif
		break;

	case 8:
		__bis_b(DIVM1 | DIVM0, BCSCTL2);
		break;
	}
}

static inline void
clock_smclk_source_lfxt1(void) { __bis_b(SELS, BCSCTL2); }
static inline void
clock_smclk_source_vlo(void)   { __bis_b(SELS, BCSCTL2); }
static inline void
clock_smclk_source_dco(void)   { __bic_b(SELS, BCSCTL2); }

static inline void
clock_smclk_divide(unsigned int n)
{
	switch (n) {
	case 1:
		__bic_b(DIVS1 | DIVS0, BCSCTL2);
		break;

	case 2:
#ifdef ATOMIC
		BCSCTL2 = (BCSCTL2 & ~DIVS1) | DIVS0;
#else
		__bic_b(DIVS1, BCSCTL2);
		__bis_b(DIVS0, BCSCTL2);
#endif
		break;

	case 4:
#ifdef ATOMIC
		BCSCTL2 = (BCSCTL2 & ~DIVS0) | DIVS1;
#else
		__bis_b(DIVS1, BCSCTL2);
		__bic_b(DIVS0, BCSCTL2);
#endif
		break;

	case 8:
		__bis_b(DIVS1 | DIVS0, BCSCTL2);
		break;
	}
}

#ifdef DCOR
static inline void
clock_dco_resistor_external(void) { __bis_b(DCOR, BCSCTL2); }
static inline void
clock_dco_resistor_internal(void) { __bic_b(DCOR, BCSCTL2); }
#endif

static inline void
clock_xt2_range_0_4_to_1MHz(void) { __bic_b(XT2S1 | XT2S0, BCSCTL3); }
static inline void
clock_xt2_range_1_to_3MHz(void)   { BCSCTL3 = (BCSCTL3 & ~XT2S1) | XT2S0; }
static inline void
clock_xt2_range_3_to_16MHz(void)  { BCSCTL3 = (BCSCTL3 & ~XT2S0) | XT2S1; }
static inline void
clock_xt2_digital(void)           { __bis_b(XT2S0 | XT2S1, BCSCTL3); }

#endif
