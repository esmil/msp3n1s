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

#include <io.h>

static inline void
clock_init_1MHz()
{
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL  = CALDCO_1MHZ;
}

static inline void
clock_mclk_1MHz()
{
	BCSCTL1 = (BCSCTL1 & ~(RSEL3 | RSEL2 | RSEL1 | RSEL0)) | CALBC1_1MHZ;
	DCOCTL  = CALDCO_1MHZ;
}

static inline void
clock_xt2_off()              { BCSCTL1 |= XT2OFF; }
static inline void
clock_xt2_on()               { BCSCTL1 &= ~XT2OFF; }
static inline void
clock_lfxt1_mode_high_freq() { BCSCTL1 |= XTS; }
static inline void
clock_lfxt1_mode_low_freq()  { BCSCTL1 &= ~XTS; }

static inline void
clock_aclk_divide(unsigned int n)
{
	switch (n) {
	case 1:
		BCSCTL1 &= ~(DIVA1 | DIVA0);
		break;

	case 2:
#ifdef ATOMIC
		BCSCTL1 = (BCSCTL1 & ~DIVA1) | DIVA0;
#else
		BCSCTL1 &= ~DIVA1;
		BCSCTL1 |= DIVA0;
#endif
		break;

	case 4:
#ifdef ATOMIC
		BCSCTL1 = (BCSCTL1 & ~DIVA0) | DIVA1;
#else
		BCSCTL1 |= DIVA1;
		BCSCTL1 &= ~DIVA0;
#endif
		break;

	case 8:
		BCSCTL1 |= DIVA1 | DIVA0;
		break;
	}
}

static inline void
clock_mclk_source_dco()   { BCSCTL2 &= ~(SELM1 | SELM0); }
static inline void
clock_mclk_source_xt2()   { BCSCTL2 = (BCSCTL2 & ~SELM0) | SELM0; }
static inline void
clock_mclk_source_lfxt1() { BCSCTL2 |= SELM1 | SELM0; }
static inline void
clock_mclk_source_vlo()   { BCSCTL2 |= SELM1 | SELM0; }

static inline void
clock_mclk_divide(unsigned int n)
{
	switch (n) {
	case 1:
		BCSCTL2 &= ~(DIVM1 | DIVM0);
		break;

	case 2:
#ifdef ATOMIC
		BCSCTL2 = (BCSCTL2 & ~DIVM1) | DIVM0;
#else
		BCSCTL2 &= ~DIVM1;
		BCSCTL2 |= DIVM0;
#endif
		break;

	case 4:
#ifdef ATOMIC
		BCSCTL2 = (BCSCTL2 & ~DIVM0) | DIVM1;
#else
		BCSCTL2 |= DIVM1;
		BCSCTL2 &= ~DIVM0;
#endif
		break;

	case 8:
		BCSCTL2 |= DIVM1 | DIVM0;
		break;
	}
}

static inline void
clock_smclk_source_lfxt1() { BCSCTL2 |= SELS; }
static inline void
clock_smclk_source_vlo()   { BCSCTL2 |= SELS; }
static inline void
clock_smclk_source_dco()   { BCSCTL2 &= ~SELS; }

static inline void
clock_smclk_divide(unsigned int n)
{
	switch (n) {
	case 1:
		BCSCTL2 &= ~(DIVS1 | DIVS0);
		break;

	case 2:
#ifdef ATOMIC
		BCSCTL2 = (BCSCTL2 & ~DIVS1) | DIVS0;
#else
		BCSCTL2 &= ~DIVS1;
		BCSCTL2 |= DIVS0;
#endif
		break;

	case 4:
#ifdef ATOMIC
		BCSCTL2 = (BCSCTL2 & ~DIVS0) | DIVS1;
#else
		BCSCTL2 |= DIVS1;
		BCSCTL2 &= ~DIVS0;
#endif
		break;

	case 8:
		BCSCTL2 |= DIVS1 | DIVS0;
		break;
	}
}

#ifdef DCOR
static inline void
clock_dco_resistor_external() { BCSCTL2 |= DCOR; }
static inline void
clock_dco_resistor_internal() { BCSCTL2 &= ~DCOR; }
#endif

static inline void
clock_xt2_range_0_4_to_1MHz() { BCSCTL3 &= ~(XT2S1 | XT2S0); }
static inline void
clock_xt2_range_1_to_3MHz()   { BCSCTL3 = (BCSCTL3 & ~XT2S1) | XT2S0; }
static inline void
clock_xt2_range_3_to_16MHz()  { BCSCTL3 = (BCSCTL3 & ~XT2S0) | XT2S1; }
static inline void
clock_xt2_digital()           { BCSCTL3 |= XT2S0 | XT2S1; }

#endif
