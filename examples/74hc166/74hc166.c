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

#include <watchdog.h>
#include <clock.h>
#include <pins.h>

#define LED1 1.0
#define LED2 1.6
#define S2   1.3

#define SHIFTIN_CLOCK 1.4
#define SHIFTIN_DATA  1.5
#define PE 1.7

#include "lib/serial_tx.c"

void
port1_interrupt(void)
{
	pin_interrupt_disable(S2);
	LPM0_EXIT;
}

#include "lib/shiftin.c"

int
main(void)
{
	watchdog_off();
	clock_init_1MHz();

	/* set SHIFTIN_DATA and S2 in input mode and the rest in output mode */
	port1_direction = ~(pin_mask(SHIFTIN_DATA) | pin_mask(S2));

	/* set LED1 SHIFTIN_CLOCK and PE low, and the rest high */
	port1_output = ~(pin_mask(LED1) |
	                 pin_mask(SHIFTIN_CLOCK) |
			 pin_mask(PE));

	/* enable pull-up on S2 */
	pin_resistor_enable(S2);

	/* initialize serial output */
	serial_init_clock();
	serial_init_tx();

	/* enable interrupts */
	__eint();

	while (1) {
		unsigned char byte;

		/* wait for button press */
		pin_low(LED2);
		pin_interrupt_clear(S2);
		pin_interrupt_enable(S2);
		LPM0;
		pin_high(LED2);

		pin_high(SHIFTIN_CLOCK);
		pin_high(PE);
		byte = shiftin_byte_msb();
		pin_low(PE);

		serial_puts("Read: ");
		serial_dump(&byte, 1);
	}
}
