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

#define S2 1.3

#include "lib/oneslave.h"

/* data pins must be 1.4, 1.5, 1.6 and 1.7,
 * but these can be freely chosen among the rest */
#define LCD_RS 1.0
#define LCD_EN 1.1
#include "lib/lcd.c"

static unsigned char buf[16];

const unsigned char onewire_rom[8] = {
	0xFE, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C
};

struct onewire_command onewire_commands[] = {
	{
		.cmd = 'w',
		.flags = ONEWIRE_WAKEUP | ONEWIRE_WRITE,
		.len = sizeof(buf),
		.data = buf,
	},
	{
		.cmd = 'c',
		.flags = ONEWIRE_WAKEUP | ONEWIRE_WRITE,
		.len = 1,
		.data = buf,
	},
	{
		.cmd = 's',
		.flags = ONEWIRE_WAKEUP,
	},
	{
		.cmd = 'x',
		.flags = ONEWIRE_WAKEUP | ONEWIRE_LAST,
	},
};

unsigned char onewire_alarm;

void
port1_interrupt(void)
{
	onewire_alarm = 1;
	pin_interrupt_disable(S2);
}

int
main(void)
{
	watchdog_off();
	clock_init_1MHz();

	/* initialize onewire pin */
	pin_mode_input(ONESLAVE_PIN);
	pin_low(ONESLAVE_PIN);
	pin_function_primary(ONESLAVE_PIN);

	lcd_init();

	/* see fx.
	 * http://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller
	 * for more commands
	 */
	lcd_command(0x28); /* 2 lines, 5x8 dot font */
	lcd_command(0x10); /* move cursor on write */
	lcd_command(0x0C); /* cursor off */
	lcd_command(0x06); /* move cursor right */
	lcd_clear();       /* clear display */
	lcd_puts("Display ready...");

	/* initialize S2 to receive button press interrupt */
	pin_mode_input(S2);
	pin_high(S2);
	pin_resistor_enable(S2);
	pin_interrupt_falling(S2);
	pin_interrupt_enable(S2);

	/* enable interrupts */
	__eint();

	while (1) {
		unsigned int i;

		switch (onewire_getcmd()) {
		case 'w':
			pin_high(LCD_RS);
			for (i = 0; i < sizeof(buf); i++) {
				unsigned char c = buf[i];

				if (c == '\0')
					break;

				lcd_send(c);
			}
			break;

		case 'c':
			lcd_command(buf[0]);
			break;

		case 's':
			lcd_sync();
			lcd_command(0x28); /* 2 lines, 5x8 dot font */
			break;

		case 'x':
			onewire_alarm = 0;
			pin_interrupt_clear(S2);
			pin_interrupt_enable(S2);
		}
	}
}
