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

#ifndef _ADC10_H
#define _ADC10_H

#include <msp430.h>
#include <macros.h>

static inline void
adc10_reference_neg_external(void) { __bis_w(SREF2, ADC10CTL0); }
static inline void
adc10_reference_neg_ground(void)   { __bic_w(SREF2, ADC10CTL0); }

static inline void
adc10_reference_pos_Vcc(void)
{
	__bic_w(SREF1 | SREF0, ADC10CTL0);
}

static inline void
adc10_reference_pos_Vref(void)
{
#ifdef ATOMIC
	ADC10CTL0 = (ADC10CTL0 & ~SREF1) | SREF0;
#else
	__bic_w(SREF1, ADC10CTL0);
	__bis_w(SREF0, ADC10CTL0);
#endif
}

static inline void
adc10_reference_pos_external(void)
{
#ifdef ATOMIC
	ADC10CTL0 = (ADC10CTL0 & ~SREF0) | SREF1;
#else
	__bis_w(SREF1, ADC10CTL0);
	__bic_w(SREF0, ADC10CTL0);
#endif
}

static inline void
adc10_reference_pos_buffered(void)
{
	__bis_w(SREF1 | SREF0, ADC10CTL0);
}

static inline void
adc10_sample_and_hold_clocks(unsigned int n)
{
	switch (n) {
	case 4:
		__bic_w(ADC10SHT1 | ADC10SHT0, ADC10CTL0);
		break;
	case 8:
#ifdef ATOMIC
		ADC10CTL0 = (ADC10CTL0 & ~ADC10SHT1) | ADC10SHT0;
#else
		__bic_w(ADC10SHT1, ADC10CTL0);
		__bis_w(ADC10SHT0, ADC10CTL0);
#endif
		break;
	case 16:
#ifdef ATOMIC
		ADC10CTL0 = (TACTL & ~ADC10SHT0) | ADC10SHT1;
#else
		__bis_w(ADC10SHT1, ADC10CTL0);
		__bic_w(ADC10SHT0, ADC10CTL0);
#endif
		break;
	case 64:
		__bis_w(ADC10SHT1 | ADC10SHT0, ADC10CTL0);
		break;
	}
}

static inline void
adc10_sample_rate_50ksps(void)       { __bis_w(ADC10SR, ADC10CTL0); }
static inline void
adc10_sample_rate_200ksps(void)      { __bic_w(ADC10SR, ADC10CTL0); }
static inline void
adc10_reference_output_enable(void)  { __bis_w(REFOUT, ADC10CTL0); }
static inline void
adc10_reference_output_disable(void) { __bic_w(REFOUT, ADC10CTL0); }
static inline void
adc10_reference_burst_enable(void)   { __bis_w(REFBURST, ADC10CTL0); }
static inline void
adc10_reference_burst_disable(void)  { __bic_w(REFBURST, ADC10CTL0); }
static inline void
adc10_sample_multiple(void)          { __bis_w(MSC, ADC10CTL0); }
static inline void
adc10_sample_single(void)            { __bic_w(MSC, ADC10CTL0); }
static inline void
adc10_reference_2_5V(void)           { __bis_w(REF2_5V, ADC10CTL0); }
static inline void
adc10_reference_1_5V(void)           { __bic_w(REF2_5V, ADC10CTL0); }
static inline void
adc10_reference_enable(void)         { __bis_w(REFON, ADC10CTL0); }
static inline void
adc10_reference_disable(void)        { __bic_w(REFON, ADC10CTL0); }
static inline void
adc10_interrupt_enable(void)         { __bis_w(ADC10IE, ADC10CTL0); }
static inline void
adc10_interrupt_disable(void)        { __bic_w(ADC10IE, ADC10CTL0); }
static inline int
adc10_interrupt_enabled(void)        { return ADC10CTL0 & ADC10IE; }
static inline void
adc10_interrupt_raise(void)          { __bis_w(ADC10IFG, ADC10CTL0); }
static inline void
adc10_interrupt_clear(void)          { __bic_w(ADC10IFG, ADC10CTL0); }
static inline int
adc10_interrupt_flag(void)           { return ADC10CTL0 & ADC10IFG; }
static inline void
adc10_conversion_enable(void)        { __bis_w(ENC, ADC10CTL0); }
static inline void
adc10_conversion_disable(void)       { __bic_w(ENC, ADC10CTL0); }
static inline void
adc10_conversion_start(void)         { __bis_w(ADC10SC, ADC10CTL0); }
static inline void
adc10_conversion_run(void)           { ADC10CTL0 |= ENC | ADC10SC; }

static inline void
adc10_sample_and_hold_source_software(void)
{
	__bic_w(SHS1 | SHS0, ADC10CTL1);
}

static inline void
adc10_sample_and_hold_source_timera_out0(void)
{
#ifdef ATOMIC
	ADC10CTL1 = (ADC10CTL1 & ~SHS1) | SHS0;
#else
	__bic_w(SHS1, ADC10CTL1);
	__bis_w(SHS0, ADC10CTL1);
#endif
}

static inline void
adc10_sample_and_hold_source_timera_out1(void)
{
#ifdef ATOMIC
	ADC10CTL1 = (ADC10CTL1 & ~SHS0) | SHS1;
#else
	__bis_w(SHS1, ADC10CTL1);
	__bic_w(SHS0, ADC10CTL1);
#endif
}

static inline void
adc10_sample_and_hold_source_timera_out2(void)
{
	__bis_w(SHS1 | SHS0, ADC10CTL1);
}

static inline void
adc10_data_format_2s_complement(void)     { __bis_w(ADC10DF, ADC10CTL1); }
static inline void
adc10_data_format_binary(void)            { __bic_w((ADC10DF), ADC10CTL1); }
static inline void
adc10_sample_and_hold_signal_invert(void) { __bis_w(ISSH, ADC10CTL1); }
static inline void
adc10_sample_and_hold_signal_normal(void) { __bic_w(ISSH, ADC10CTL1); }

static inline void
adc10_clock_source_osc(void)
{
	__bic_w(ADC10SSEL1 | ADC10SSEL0, ADC10CTL1);
}

static inline void
adc10_clock_source_aclk(void)
{
#ifdef ATOMIC
	ADC10CTL1 = (ADC10CTL1 & ~ADC10SSEL1) | ADC10SSEL0;
#else
	__bic_w(ADC10SSEL1, ADC10CTL1);
	__bis_w(ADC10SSEL0, ADC10CTL1);
#endif
}

static inline void
adc10_clock_source_mclk(void)
{
#ifdef ATOMIC
	ADC10CTL1 = (ADC10CTL1 & ~ADC10SSEL0) | ADC10SSEL1;
#else
	__bis_w(ADC10SSEL1, ADC10CTL1);
	__bic_w(ADC10SSEL0, ADC10CTL1);
#endif
}

static inline void
adc10_clock_source_smclk(void)
{
	__bis_w(ADC10SSEL1 | ADC10SSEL0, ADC10CTL1);
}

static inline void
adc10_channel_multiple(void) { __bis_w(CONSEQ0, ADC10CTL1); }
static inline void
adc10_channel_single(void)   { __bic_w(CONSEQ0, ADC10CTL1); }
static inline void
adc10_sequence_repeat(void)  { __bis_w(CONSEQ1, ADC10CTL1); }
static inline void
adc10_sequence_single(void)  { __bic_w(CONSEQ1, ADC10CTL1); }

static inline int
adc10_busy(void)             { return ADC10CTL1 & ADC10BUSY; }

static inline void
adc10_input_enable(unsigned int n)
{
	if (n < 8) {
		__bis_b(1 << n, ADC10AE0);
		return;
	}

#ifdef ADC10AE1
	__bis_b(1 << (n-8), ADC10AE1);
#endif
}

static inline void
adc10_input_disable(unsigned int n)
{
	if (n < 8) {
		__bic_b(1 << n, ADC10AE0);
		return;
	}

#ifdef ADC10AE1
	__bic_b(1 << (n-8), ADC10AE1);
#endif
}

static inline int
adc10_result(void)                   { return ADC10MEM; }

static inline void
adc10_transfer_two_block(void)       { __bis_b(ADC10TB, ADC10DTC0); }
static inline void
adc10_transfer_one_block(void)       { __bic_b(ADC10TB, ADC10DTC0); }
static inline void
adc10_transfer_continuous(void)      { __bis_b(ADC10CT, ADC10DTC0); }
static inline void
adc10_transfer_intermittent(void)    { __bic_b(ADC10CT, ADC10DTC0); }
static inline int
adc10_transfer_block1(void)          { return ADC10DTC0 & ADC10B1; }
static inline int
adc10_transfer_block2(void)          { return !(ADC10DTC0 & ADC10B1); }

#define adc10_interrupt(x...) __attribute__((interrupt(ADC10_VECTOR)))\
	adc10_interrupt(x)

#endif
