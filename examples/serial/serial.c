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

/*
 * This "parrot program" just echoes back whatever is send
 * to it over the serial line. Type
 *
 * $ make screen
 *
 * to test it (and "<Ctrl-A>:quit<Enter>" to exit screen).
 */

#include <watchdog.h>
#include <clock.h>
#include <pins.h>

#define LED1 1.0
#define LED2 1.6

#include "lib/serial_tx.c"

#define SERIAL_RX_BUFSIZE 32
#include "lib/serial_rx_buffered.c"

int
main(void)
{
	watchdog_off();
	clock_init_1MHz();

	/* set all pins to output high */
	port1_direction = 0xFF;
	port1_output = 0xFF;

	/* initialize serial clock, tx and rx parts */
	serial_init_clock();
	serial_init_tx();
	serial_init_rx();

	/* enable interrupts */
	__eint();

	pin_low(LED1);
	pin_high(LED2);

	while (1) {
		unsigned char c;

		pin_toggle(LED1);
		pin_toggle(LED2);

		c = serial_getchar();
		serial_putchar(c);
		if (c == '\r')
			serial_putchar('\n');
	}
}
