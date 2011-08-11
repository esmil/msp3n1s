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

#include <stdlib.h>
#include <watchdog.h>
#include <clock.h>
#include <pins.h>
#include <usi.h>
#include <delay.h>

#define LED1 1.0
#define S2   1.3

#include "lib/i2c_master.c"

static void
port1_interrupt(void)
{
	pin_interrupt_disable(S2);
	LPM0_EXIT;
}

int
main(void)
{
	watchdog_off();
	clock_init_1MHz();

	/* set all pins to output high */
	port1_direction = 0xFF;
	port1_output = 0xFF;

	/* configure push button */
	pin_mode_input(S2);
	pin_high(S2);
	pin_resistor_enable(S2);
	pin_interrupt_falling(S2);

	usi_i2c_clock_standard();
	usi_i2c_master_init();

	pin_low(LED1);

	/* enable interrupts */
	__eint();

	for (unsigned char c = 0;; c++) {
		/* wait for keypress */
		pin_interrupt_clear(S2);
		pin_interrupt_enable(S2);
		LPM0;

		pin_high(LED1);
		if (i2c_start(32, 0))
			goto fast;

		if (i2c_send_byte(c))
			goto slow;

		i2c_stop(1);
		pin_low(LED1);
	}

fast:
	while (1) {
		pin_low(LED1);
		delay(30000);
		pin_high(LED1);
		delay(30000);
	}

slow:
	while (1) {
		pin_low(LED1);
		delay(0xFFFF);
		pin_high(LED1);
		delay(0xFFFF);
	}
}
