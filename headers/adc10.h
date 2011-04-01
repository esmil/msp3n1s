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

static inline void
adc10_reference_neg_external() { ADC10CTL0 |= SREF2; }
static inline void
adc10_reference_neg_ground()   { ADC10CTL0 &= ~SREF2; }

static inline void
adc10_reference_pos_Vcc()
{
	ADC10CTL0 &= ~(SREF1 | SREF0);
}

static inline void
adc10_reference_pos_Vref()
{
#ifdef ATOMIC
	ADC10CTL0 = (ADC10CTL0 & ~SREF1) | SREF0;
#else
	ADC10CTL0 &= ~SREF1;
	ADC10CTL0 |= SREF0;
#endif
}

static inline void
adc10_reference_pos_external()
{
#ifdef ATOMIC
	ADC10CTL0 = (ADC10CTL0 & ~SREF0) | SREF1;
#else
	ADC10CTL0 |= SREF1;
	ADC10CTL0 &= ~SREF0;
#endif
}

static inline void
adc10_reference_pos_buffered()
{
	ADC10CTL0 |= SREF1 | SREF0;
}

static inline void
adc10_sample_and_hold_clocks(unsigned int n)
{
	switch (n) {
	case 4:
		ADC10CTL0 &= ~(ADC10SHT1 | ADC10SHT0);
		break;
	case 8:
#ifdef ATOMIC
		ADC10CTL0 = (ADC10CTL0 & ~ADC10SHT1) | ADC10SHT0;
#else
		ADC10CTL0 &= ~ADC10SHT1;
		ADC10CTL0 |= ADC10SHT0;
#endif
		break;
	case 16:
#ifdef ATOMIC
		ADC10CTL0 = (TACTL & ~ADC10SHT0) | ADC10SHT1;
#else
		ADC10CTL0 |= ADC10SHT1;
		ADC10CTL0 &= ~ADC10SHT0;
#endif
		break;
	case 64:
		ADC10CTL0 |= ADC10SHT1 | ADC10SHT0;
		break;
	}
}

static inline void
adc10_sampling_rate_50ksps()     { ADC10CTL0 |= ADC10SR; }
static inline void
adc10_sampling_rate_200ksps()    { ADC10CTL0 &= ~ADC10SR; }
static inline void
adc10_reference_output_enable()  { ADC10CTL0 |= REFOUT; }
static inline void
adc10_reference_output_disable() { ADC10CTL0 &= ~REFOUT; }
static inline void
adc10_reference_burst_enable()   { ADC10CTL0 |= REFBURST; }
static inline void
adc10_reference_burst_disable()  { ADC10CTL0 &= ~REFBURST; }
static inline void
adc10_sample_multiple()          { ADC10CTL0 |= MSC; }
static inline void
adc10_sample_single()            { ADC10CTL0 &= ~MSC; }
static inline void
adc10_reference_2_5V()           { ADC10CTL0 |= REF2_5V; }
static inline void
adc10_reference_1_5V()           { ADC10CTL0 &= ~REF2_5V; }
static inline void
adc10_reference_enable()         { ADC10CTL0 |= REFON; }
static inline void
adc10_reference_disable()        { ADC10CTL0 &= ~REFON; }
static inline void
adc10_interrupt_enable()         { ADC10CTL0 |= ADC10IE; }
static inline void
adc10_interrupt_disable()        { ADC10CTL0 &= ~ADC10IE; }
static inline int
adc10_interrupt_enabled()        { return ADC10CTL0 & ADC10IE; }
static inline void
adc10_interrupt_raise()          { ADC10CTL0 |= ADC10IFG; }
static inline void
adc10_interrupt_clear()          { ADC10CTL0 &= ~ADC10IFG; }
static inline int
adc10_interrupt_flag()           { return ADC10CTL0 & ADC10IFG; }
static inline void
adc10_conversion_enable()        { ADC10CTL0 |= ENC; }
static inline void
adc10_conversion_disable()       { ADC10CTL0 &= ~ENC; }
static inline void
adc10_conversion_start()         { ADC10CTL0 |= ADC10SC; }
static inline void
adc10_conversion_run()           { ADC10CTL0 |= ENC | ADC10SC; }

static inline void
adc10_sample_and_hold_source_software()
{
	ADC10CTL1 &= ~(SHS1 | SHS0);
}

static inline void
adc10_sample_and_hold_source_timer_a_out0()
{
#ifdef ATOMIC
	ADC10CTL1 = (ADC10CTL1 & ~SHS1) | SHS0;
#else
	ADC10CTL1 &= ~SHS1;
	ADC10CTL1 |= SHS0;
#endif
}

static inline void
adc10_sample_and_hold_source_timer_a_out1()
{
#ifdef ATOMIC
	ADC10CTL1 = (ADC10CTL1 & ~SHS0) | SHS1;
#else
	ADC10CTL1 |= SHS1;
	ADC10CTL1 &= ~SHS0;
#endif
}

static inline void
adc10_sample_and_hold_source_timer_a_out2()
{
	ADC10CTL1 |= SHS1 | SHS0;
}

static inline void
adc10_data_format_2s_complement()     { ADC10CTL1 |= ADC10DF; }
static inline void
adc10_data_format_binary()            { ADC10CTL1 &= ~(ADC10DF); }
static inline void
adc10_sample_and_hold_signal_invert() { ADC10CTL1 |= ISSH; }
static inline void
adc10_sample_and_hold_signal_normal() { ADC10CTL1 &= ~ISSH; }

static inline void
adc10_clock_source_osc()
{
	ADC10CTL1 &= ~(ADC10SSEL1 | ADC10SSEL0);
}

static inline void
adc10_clock_source_aclk()
{
#ifdef ATOMIC
	ADC10CTL1 = (ADC10CTL1 & ~ADC10SSEL1) | ADC10SSEL0;
#else
	ADC10CTL1 &= ~ADC10SSEL1;
	ADC10CTL1 |= ADC10SSEL0;
#endif
}

static inline void
adc10_clock_source_mclk()
{
#ifdef ATOMIC
	ADC10CTL1 = (ADC10CTL1 & ~ADC10SSEL0) | ADC10SSEL1;
#else
	ADC10CTL1 |= ADC10SSEL1;
	ADC10CTL1 &= ~ADC10SSEL0;
#endif
}

static inline void
adc10_clock_source_smclk()
{
	ADC10CTL1 |= ADC10SSEL1 | ADC10SSEL0;
}

static inline void
adc10_channel_multiple() { ADC10CTL1 |= CONSEQ0; }
static inline void
adc10_channel_single()   { ADC10CTL1 &= ~CONSEQ0; }
static inline void
adc10_sequence_repeat()  { ADC10CTL1 |= CONSEQ1; }
static inline void
adc10_sequence_single()  { ADC10CTL1 &= ~CONSEQ1; }

static inline int
adc10_busy()             { return ADC10CTL1 & ADC10BUSY; }

static inline void
adc10_input_enable(unsigned int n)
{
	if (n < 8) {
		ADC10AE0 |= 1 << n;
		return;
	}

#ifdef ADC10AE1
	ADC10AE1 |= 1 << (n-8);
#endif
}

static inline void
adc10_input_disable(unsigned int n)
{
	if (n < 8) {
		ADC10AE0 &= ~(1 << n);
		return;
	}

#ifdef ADC10AE1
	ADC10AE1 &= ~(1 << (n-8));
#endif
}

static inline int
adc10_result()                   { return ADC10MEM; }

static inline void
adc10_transfer_two_block()       { ADC10DTC0 |= ADC10TB; }
static inline void
adc10_transfer_one_block()       { ADC10DTC0 &= ~ADC10TB; }
static inline void
adc10_transfer_continuous()      { ADC10DTC0 |= ADC10CT; }
static inline void
adc10_transfer_intermittent()    { ADC10DTC0 &= ~ADC10CT; }
static inline int
adc10_transfer_block1()          { return ADC10DTC0 & ADC10B1; }
static inline int
adc10_transfer_block2()          { return !(ADC10DTC0 & ADC10B1); }

#define adc10_interrupt() interrupt(ADC10_VECTOR) adc10_interrupt()

#endif
