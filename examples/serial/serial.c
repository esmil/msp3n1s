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
#include <clock.h>
#include <pins.h>
#include <watchdog.h>

#define LED1 1.0
#define LED2 1.6
#define S2   1.3

#define SERIAL_PRINTF_X
#define SERIAL_PRINTF_D
#define SERIAL_PRINTF_S
#include "lib/serial.c"

#define BLINKTIME 0xFFFF

port1_interrupt()
{
	pin_interrupt_disable(S2); /* debounce */
	pin_interrupt_clear(S2);

	watchdog_timer_8ms();
	watchdog_timer_interrupt_clear();
	watchdog_timer_interrupt_enable();

	LPM0_EXIT;
}

watchdog_interrupt()
{
	watchdog_timer_interrupt_disable();
	watchdog_timer_interrupt_clear();
	watchdog_off();

	pin_interrupt_enable(S2); /* debouncing complete */
}

timera_cc1_interrupt()
{
	timera_cc1_add(BLINKTIME);

	pin_toggle(LED1);
	pin_toggle(LED2);

	timera_cc1_interrupt_clear();
}

int
main()
{
	watchdog_off();
	clock_init_1MHz();

	/* set all pins in output mode */
	port1_direction_set(0xFF);

	/* configure push button */
	pin_mode_input(S2);
	pin_high(S2);
	pin_resistor_enable(S2);
	pin_interrupt_falling(S2);
	pin_interrupt_clear(S2);
	pin_interrupt_enable(S2);

	serial_init();

	/* enable interrupts */
	eint();

	/* start blinking */
	pin_high(LED1);
	pin_low(LED2);
	timera_cc1_set(timera_count() + BLINKTIME);
	timera_cc1_interrupt_enable();

	while (1) {
		LPM0; /* wait for button press */
#if 0
		serial_puts("Hello world!\n");
#else
		serial_printf("Hello %s, %x %d!\n", "world", 17, -309);
#endif
	}
}
