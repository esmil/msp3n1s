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

#ifndef _USI_H
#define _USI_H

#include <msp430.h>
#include <macros.h>

static inline void
usi_pin7_enable(void)    { __bis_b(USICTL0, USIPE7); }
static inline void
usi_pin7_disable(void)   { __bic_b(USICTL0, USIPE7); }
static inline void
usi_pin6_enable(void)    { __bis_b(USICTL0, USIPE6); }
static inline void
usi_pin6_disable(void)   { __bic_b(USICTL0, USIPE6); }
static inline void
usi_pin5_enable(void)    { __bis_b(USICTL0, USIPE5); }
static inline void
usi_pin5_disable(void)   { __bic_b(USICTL0, USIPE5); }
static inline void
usi_lsb(void)            { __bis_b(USICTL0, USILSB); }
static inline void
usi_msb(void)            { __bic_b(USICTL0, USILSB); }
static inline void
usi_master(void)         { __bis_b(USICTL0, USIMST); }
static inline void
usi_slave(void)          { __bic_b(USICTL0, USIMST); }
static inline void
usi_latch_enable(void)   { __bis_b(USICTL0, USIGE); }
static inline void
usi_latch_disable(void)  { __bic_b(USICTL0, USIGE); }
static inline void
usi_output_enable(void)  { __bis_b(USICTL0, USIOE); }
static inline void
usi_output_disable(void) { __bic_b(USICTL0, USIOE); }
static inline void
usi_reset(void)          { __bis_b(USICTL0, USISWRST); }
static inline void
usi_release(void)        { __bic_b(USICTL0, USISWRST); }

static inline void
usi_output_and_latch_enable(void)  { __bis_b(USICTL0, USIOE | USIGE); }
static inline void
usi_output_and_latch_disable(void) { __bic_b(USICTL0, USIOE | USIGE); }

static inline void
usi_clock_edge_first(void)          { __bis_b(USICTL1, USICKPH); }
static inline void
usi_clock_edge_second(void)         { __bic_b(USICTL1, USICKPH); }
static inline void
usi_i2c_enable(void)                { __bis_b(USICTL1, USII2C); }
static inline void
usi_i2c_disable(void)               { __bic_b(USICTL1, USII2C); }
static inline void
usi_start_interrupt_enable(void)    { __bis_b(USICTL1, USISTTIE); }
static inline void
usi_start_interrupt_disable(void)   { __bic_b(USICTL1, USISTTIE); }
static inline void
usi_counter_interrupt_enable(void)  { __bis_b(USICTL1, USIIE); }
static inline void
usi_counter_interrupt_disable(void) { __bic_b(USICTL1, USIIE); }
static inline unsigned char
usi_arbitration_lost(void)          { return USICTL1 & USIAL; }
static inline void
usi_stop_raise(void)                { __bis_b(USICTL1, USISTP); }
static inline void
usi_stop_clear(void)                { __bic_b(USICTL1, USISTP); }
static inline unsigned char
usi_stop_flag(void)                 { return USICTL1 & USISTP; }
static inline void
usi_start_interrupt_raise(void)     { __bis_b(USICTL1, USISTTIFG); }
static inline void
usi_start_interrupt_clear(void)     { __bic_b(USICTL1, USISTTIFG); }
static inline unsigned char
usi_start_interrupt_flag(void)      { return USICTL1 & USISTTIFG; }
static inline void
usi_counter_interrupt_raise(void)   { __bis_b(USICTL1, USIIFG); }
static inline void
usi_counter_interrupt_clear(void)   { __bic_b(USICTL1, USIIFG); }
static inline unsigned char
usi_counter_interrupt_flag(void)    { return USICTL1 & USIIFG; }

static inline void
usi_clock_source_sclk(void)
{
	USICKCTL &= ~(USISSEL2 | USISSEL1 | USISSEL0);
}

static inline void
usi_clock_source_aclk(void)
{
	USICKCTL = (USICKCTL & ~(USISSEL2 | USISSEL1)) | USISSEL0;
}

static inline void
usi_clock_inactive_high(void) { __bis_b(USICKCTL, USICKPL); }
static inline void
usi_clock_inactive_low(void)  { __bic_b(USICKCTL, USICKPL); }
static inline void
usi_clock_input_low(void)     { __bis_b(USICKCTL, USISWCLK); }
static inline void
usi_clock_input_high(void)    { __bic_b(USICKCTL, USISWCLK); }

static inline void
usi_counter_set(unsigned char bits) { __bis_b(USICNT, bits); }
static inline void
usi_counter_release(void)           { __bis_b(USICNT, USISCLREL); }
static inline void
usi_counter_grab(void)              { __bic_b(USICNT, USISCLREL); }

/*
 * I2C
 */
static inline void
usi_i2c_master_init(void)
{
	P1OUT |= 0xC0;
	P1REN |= 0xC0;
	USICTL0 = USIPE7 | USIPE6 | USIMST;
	USICTL1 = USII2C | USIIFG;
}

static inline void
usi_i2c_clock_fast(void)
{
	USICKCTL = USIDIV0 | USISSEL1 | USICKPL;
}

static inline void
usi_i2c_clock_standard(void)
{
	USICKCTL = USIDIV2 | USISSEL1 | USICKPL;
}

static inline void
usi_i2c_clock_slow(void)
{
	USICKCTL = USIDIV2 | USIDIV1 | USIDIV0 | USISSEL1 | USICKPL;
}

/*
 * SPI
 */
static inline void
usi_spi_master_init(void)
{
	USICTL0 = USIPE7 | USIPE6 | USIPE5 | USIMST;
	USICTL1 = USICKPH;
}

static inline void
usi_spi_clock_d1(void)   { USICKCTL = USISSEL1; }
static inline void
usi_spi_clock_d2(void)   { USICKCTL = USISSEL1 | USIDIV0; }
static inline void
usi_spi_clock_d4(void)   { USICKCTL = USISSEL1 | USIDIV1; }
static inline void
usi_spi_clock_d8(void)   { USICKCTL = USISSEL1 | USIDIV1 | USIDIV0; }
static inline void
usi_spi_clock_d16(void)  { USICKCTL = USISSEL1 | USIDIV2; }
static inline void
usi_spi_clock_d32(void)  { USICKCTL = USISSEL1 | USIDIV2 | USIDIV0; }
static inline void
usi_spi_clock_d64(void)  { USICKCTL = USISSEL1 | USIDIV2 | USIDIV1; }
static inline void
usi_spi_clock_d128(void) { USICKCTL = USISSEL1 | USIDIV2 | USIDIV1 | USIDIV0; }

static inline void
usi_spi_slave_init(void)
{
	USICTL0 = USIPE7 | USIPE6 | USIPE5;
	USICTL1 = USICKPH;
}

#define usi_data      USISR
#define usi_data_high USISRH
#define usi_data_low  USISRL

#define usi_interrupt(x...) __attribute__((interrupt(USI_VECTOR)))\
	usi_interrupt(x)

#endif
