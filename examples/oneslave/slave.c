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

#include "lib/oneslave.h"

#define LED1 1.0
#define LED2 1.6
#define S2   1.3

static unsigned char buf[16] = "Hello, world!";

const unsigned char onewire_rom[8] = {
	0x2D, 0x54, 0xD2, 0xEF, 0x00, 0x00, 0x00, 0x2B
};

const struct onewire_command onewire_commands[] = {
	{
		.cmd = 'r',
		.len = sizeof(buf),
		.data = buf,
	},
	{
		.cmd = 'w',
		.flags = ONEWIRE_WRITE,
		.len = sizeof(buf),
		.data = buf,
	},
	{
		.cmd = 'x',
		.flags = ONEWIRE_WAKEUP,
	},
	{
		.cmd = 'A',
		.flags = ONEWIRE_WAKEUP,
	},
	{
		.cmd = 'a',
		.flags = ONEWIRE_WAKEUP,
	},
	{
		.cmd = 'B',
		.flags = ONEWIRE_WAKEUP,
	},
	{
		.cmd = 'b',
		.flags = ONEWIRE_WAKEUP | ONEWIRE_LAST,
	},
};

unsigned char onewire_alarm;

void
port1_interrupt(void)
{
	/* enter alarm mode and disable button irq */
	onewire_alarm = 1;
	pin_interrupt_disable(S2);
}

int
main(void)
{
	watchdog_off();
	clock_init_1MHz();

	/* set all pins to output high */
	port1_direction = 0xFF;
	port1_output = 0xFF;
	port2_direction = 0xFF;
	port2_output = 0xFF;

	/* initialize onewire pin */
	pin_mode_input(ONESLAVE_PIN);
	pin_low(ONESLAVE_PIN);
	pin_function_primary(ONESLAVE_PIN);

	/* initialize button pin */
	pin_mode_input(S2);
	pin_resistor_enable(S2);
	pin_interrupt_falling(S2);
	pin_interrupt_clear(S2);
	pin_interrupt_enable(S2);

	/* switch off LEDs */
	pin_low(LED1);
	pin_low(LED2);

	/* enable interrupts */
	__eint();

	while (1) {
		switch (onewire_getcmd()) {
		case 'A': pin_high(LED1); break;
		case 'a': pin_low(LED1); break;
		case 'B': pin_high(LED2); break;
		case 'b': pin_low(LED2); break;
		case 'x':
			  /* disable alarm mode */
			  onewire_alarm = 0;
			  /* ..and re-enable button irq */
			  pin_interrupt_clear(S2);
			  pin_interrupt_enable(S2);
			  break;
		}
	}
}
