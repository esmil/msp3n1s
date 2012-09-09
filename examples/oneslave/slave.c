/*
 * This file is part of msp3n1s
 * Copyright 2011-2012 Emil Renner Berthing
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

#include <stdlib.h>
#include <watchdog.h>
#include <clock.h>
#include <pins.h>
#include <timera.h>
#include <usi.h>

#define LED1 1.0
#define LED2 1.6
#define S2   1.3

#define OW   1.2

const unsigned char rom[8] = {
	0x2D, 0x54, 0xD2, 0xEF, 0x00, 0x00, 0x00, 0x2B
};

extern unsigned char onewire_getcmd(void);

int
main(void)
{
	watchdog_off();
	clock_init_1MHz();

	/* set all pins to output high */
	port1_direction = 0xFF;
	port1_output = 0xFF;

	pin_low(LED1);

	/* initialize onewire pin */
	pin_mode_input(OW);
	pin_low(OW);
	pin_function_primary(OW);

	/* initialize timera */
	timera_clock_source_smclk();
	timera_clock_divide(1);
	timera_off();
	timera_cc1_input_a();
	timera_cc1_capture_sync();
	timera_cc1_capture_falling();
	timera_cc1_mode_capture();
	timera_clear();

	/* enable interrupts */
	__eint();

	while (1) {
		unsigned int c = onewire_getcmd();

		if (c & 1)
			pin_high(LED1);
		else
			pin_low(LED1);
	}
}
