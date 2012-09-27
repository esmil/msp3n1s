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

#include "lib/serial_common.c"

static volatile int serial_rxbyte = -1;

void
timera_interrupt(void)
{
	static unsigned char mask;
	static unsigned char byte;

	timera_cc1_interrupt_clear();

	if (timera_cc1_mode_is_capture()) {
		timera_cc1 += BITTIME + BITTIME/2;
		timera_cc1_mode_compare();
		mask = 1;
		byte = 0;
		return;
	}

	if (timera_cc1_input_sync())
		byte |= mask;

	timera_cc1 += BITTIME;
	mask <<= 1;
	if (mask)
		return;

	timera_cc1_mode_capture();
	serial_rxbyte = byte;
	LPM0_EXIT;
}

static int __attribute__((unused))
serial_getchar(void)
{
	int ret;

	__dint();
	while (serial_rxbyte < 0) {
		_BIS_SR(LPM0_bits | GIE);
		__dint();
	}

	ret = serial_rxbyte;
	serial_rxbyte = -1;
	__eint();

	return ret;
}

static void
serial_init_rx(void)
{
	pin_mode_input(SERIAL_RX);
	pin_function_primary(SERIAL_RX);

	timera_cc1_config(
			TIMERA_CC_CAPTURE_FALLING |
			TIMERA_CC_CAPTURE_SYNC |
			TIMERA_CC_INPUT_A |
			TIMERA_CC_INTERRUPT_ENABLE);
}
