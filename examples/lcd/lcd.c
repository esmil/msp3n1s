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

/* data pins must be 1.4, 1.5, 1.6 and 1.7,
 * but these can be freely chosen among the rest */
#define LCD_RS 1.0
#define LCD_EN 1.1
#include "lib/lcd.c"

void
port1_interrupt(void)
{
	pin_interrupt_disable(S2);
	LPM0_EXIT;
}

int
main(void)
{
	unsigned int count;

	watchdog_off();
	clock_init_1MHz();

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
	lcd_cursor_set(0x04);
	lcd_puts("Labitat");

	lcd_command(0x04); /* move cursor left */
	count = 0;

	/* initialize S2 to receive button press interrupt */
	pin_mode_input(S2);
	pin_high(S2);
	pin_resistor_enable(S2);
	pin_interrupt_falling(S2);

	/* enable interrupts */
	__eint();

	while (1) {
		unsigned int i = count++;

		lcd_cursor_set(0x49);
		do {
			lcd_putchar('0' + i % 10);
			i /= 10;
		} while (i);

		pin_interrupt_clear(S2);
		pin_interrupt_enable(S2);
		LPM0;
	}
}
