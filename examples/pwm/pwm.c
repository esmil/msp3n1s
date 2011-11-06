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
#include <timera.h>
#include <delay.h>

#define LED1 1.0
#define LED2 1.6

int
main(void)
{
	watchdog_off();
	clock_init_1MHz();

	/* set all pins to output high */
	port1_direction = 0xFF;
	port1_output = 0xFF;

	pin_low(LED1);

	timera_cc1_mode_compare();
	pin_function_primary(LED2);
	/* set capture/compare module 1 in reset/set output mode,
	 * that is:
	 *   pin 1.6 is set high when timera_count == timera_cc0
	 *   pin 1.6 is set low  when timera_count == timera_cc1
	 */
	timera_cc1_output_mode(7);

	/* use the system clock as timer source */
	timera_clock_source_smclk();
	/* divde the timer by 1, that is run as fast as source */
	timera_clock_divide(1);
	/* count from 0 up to timera_cc0 before resetting the counter */
	timera_mode_up();

	/* count from 0 to 1024, that is a frequency of
	 * 1 MHz / 1024 = just under 1kHz. */
	timera_cc0 = 1024;

	while (1) {
		/* sweep timer_cc1 up */
		for (timera_cc1 = 1; timera_cc1 < 1024; timera_cc1 <<= 1)
			delay_cycles(50000);

		/* ..and down in exponential steps */
		do {
			timera_cc1 >>= 1;
			timera_clear();
			delay_cycles(50000);
		} while (timera_cc1 > 0);
		delay_cycles(50000);
	}
}
