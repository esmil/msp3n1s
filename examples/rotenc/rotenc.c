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

#include <signal.h>
#include <watchdog.h>
#include <clock.h>
#include <pins.h>

#define LED1  1.0
#define LED2  1.6
#define S2    1.3

#define LEFT  1.4
#define RIGHT 1.5

#include "lib/rotenc.c"

watchdog_interrupt(void)
{
	watchdog_timer_interrupt_disable();
	watchdog_timer_interrupt_clear();
	watchdog_off();

	pin_low(LED1);
	pin_low(LED2);
}

static void
blink(void)
{
	watchdog_timer_32ms();
	watchdog_timer_interrupt_enable();
}

port1_interrupt(void)
{
	port1_interrupts_clear();
	LPM0_EXIT;
}

int
main(void)
{
	watchdog_off();
	clock_init_1MHz();

	/* set up LED pins */
	pin_mode_output(LED1);
	pin_mode_output(LED2);
	pin_low(LED1);
	pin_low(LED2);

	/* set up button bins */
	pin_mode_input(LEFT);
	pin_resistor_enable(LEFT);
	pin_high(LEFT);
	pin_interrupt_enable(LEFT);
	pin_interrupt_falling(LEFT);

	pin_mode_input(RIGHT);
	pin_resistor_enable(RIGHT);
	pin_high(RIGHT);
	pin_interrupt_enable(RIGHT);
	pin_interrupt_falling(RIGHT);

	pin_mode_input(S2);
	pin_resistor_enable(S2);
	pin_high(S2);
	pin_interrupt_enable(S2);
	pin_interrupt_falling(S2);

	/* enable interrupts */
	eint();

	while (1) {
		LPM0;

		if (!pin_is_high(S2)) {
			pin_high(LED1);
			pin_high(LED2);

			while (!pin_is_high(S2));

			pin_low(LED1);
			pin_low(LED2);
		}

		switch (rotenc_getevent()) {
		case ROTENC_NONE:
			break;

		case ROTENC_LEFT:
			pin_high(LED1);
			blink();
			break;

		case ROTENC_RIGHT:
			pin_high(LED2);
			blink();
			break;
		}
	}
}
