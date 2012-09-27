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

#include <pins.h>
#include <delay.h>

#ifndef LCD_RS
#error "LCD_RS not defined"
#endif

#ifndef LCD_EN
#error "LCD_EN not defined"
#endif

static void
lcd_pulse(void)
{
	pin_low(LCD_EN);
	__nop();
	__nop();
	__nop();
	pin_high(LCD_EN);

	/* commands need > 37us to settle */
	delay_cycles(40);
}

static void
lcd_sync(void)
{
	/* make sure we are in 8-bit mode */
	pin_low(LCD_RS);
	__bic_b(P1OUT, 0xF0);
	__bis_b(P1OUT, 0x30);
	lcd_pulse();
	lcd_pulse();
	lcd_pulse();

	/* now set 4-bit mode */
	__bic_b(P1OUT, 0x10);
	lcd_pulse();
}

static void
lcd_init(void)
{
	__bis_b(P1DIR, 0xF0);
	pin_mode_output(LCD_RS);
	pin_mode_output(LCD_EN);

	pin_high(LCD_EN);

	lcd_sync();
}

static void
lcd_send(unsigned char c)
{
	__bic_b(P1OUT, 0xF0);
	__bis_b(P1OUT, c & 0xF0);
	lcd_pulse();
	__bic_b(P1OUT, 0xF0);
	__bis_b(P1OUT, c << 4);
	lcd_pulse();
}

static void
lcd_command(unsigned char command)
{
	pin_low(LCD_RS);
	lcd_send(command);
}

static void __attribute__((unused))
lcd_clear(void)
{
	lcd_command(0x01);
	delay_cycles(1600); /* delay at least 1.52ms */
}

static void __attribute__((unused))
lcd_cursor_home(void)
{
	lcd_command(0x02);
	delay_cycles(1600); /* delay at least 1.52ms */
}

static void __attribute__((unused))
lcd_cursor_set(unsigned char pos)
{
	lcd_command(0x80 | pos);
}

static void __attribute__((unused))
lcd_putchar(unsigned char c)
{
	pin_high(LCD_RS);
	lcd_send(c);
}

static void __attribute__((unused))
lcd_puts(const char *str)
{
	unsigned char c;

	pin_high(LCD_RS);
	while ((c = *str++) != '\0')
		lcd_send(c);
}
