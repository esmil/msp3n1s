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
#include <usi.h>
#include <delay.h>

#define LED1 1.0
#define S2   1.3

#define SCL 1.6
#define SDA 1.7

#define ADDRESS 104

#include "lib/serial.c"
#include "lib/i2c_master.c"

static void
port1_interrupt(void)
{
	pin_interrupt_disable(S2);
	LPM0_EXIT;
}

static void __attribute__((unused))
serial_date(unsigned char *buf)
{
	timera_cc0_set(timera_count() + 16);
	timera_cc0_interrupt_enable();

	serial__char('2');
	serial__char(buf[5] & 0x80 ? '1' : '0');
	serial__char('0' + (buf[6] >> 4));
	serial__char('0' + (buf[6] & 0x0F));
	serial__char('-');
	serial__char(buf[5] & 0x10 ? '1' : '0');
	serial__char('0' + (buf[5] & 0x0F));
	serial__char('-');
	serial__char('0' + (buf[4] >> 4));
	serial__char('0' + (buf[4] & 0x0F));
	serial__char(' ');
	serial__char('0' + ((buf[2] >> 4) & 0x03));
	serial__char('0' + (buf[2] & 0x0F));
	serial__char(':');
	serial__char('0' + (buf[1] >> 4));
	serial__char('0' + (buf[1] & 0x0F));
	serial__char(':');
	serial__char('0' + ((buf[0] >> 4) & 0x07));
	serial__char('0' + (buf[0] & 0x0F));
	serial__char('\n');

	timera_cc0_interrupt_disable();
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

	serial_init();

	usi_i2c_clock_slow();
	usi_i2c_master_init();

	pin_low(LED1);

	/* enable interrupts */
	__eint();

	for (unsigned char c = 42;; c++) {
		unsigned char buf[8];

		/* wait for keypress */
		pin_interrupt_clear(S2);
		pin_interrupt_enable(S2);
		LPM0;

#if 1
		pin_high(LED1);
		if (i2c_start(ADDRESS, 0))
			goto dark;

		if (i2c_send_byte(0x00))
			goto bright;

		i2c_stop(0);

		if (i2c_start(ADDRESS, 1))
			goto dark;

		i2c_receive(buf, sizeof(buf));

		i2c_stop(1);
		pin_low(LED1);

		serial_puts("Received: ");
		serial_dump(buf, sizeof(buf));
		serial_date(buf);
#else
		pin_high(LED1);
		if (i2c_start(ADDRESS, 0))
			goto dark;

		if (i2c_send_byte(0x00) ||
		    i2c_send_byte(0x00) ||
		    i2c_send_byte(0x13) ||
		    i2c_send_byte(0x03))
			goto dark;

		i2c_stop(0);

		if (i2c_start(ADDRESS, 0))
			goto dark;

		if (i2c_send_byte(0x04) ||
		    i2c_send_byte(0x21) ||
		    i2c_send_byte(0x08) ||
		    i2c_send_byte(0x11))
			goto dark;

		i2c_stop(0);

		if (i2c_start(ADDRESS, 0))
			goto dark;

		if (i2c_send_byte(0x07))
			goto dark;

		i2c_stop(0);

		if (i2c_start(ADDRESS, 1))
			goto dark;

		i2c_receive(buf, 1);

		i2c_stop(0);

		buf[0] |= 0xC0;

		if (i2c_start(ADDRESS, 0))
			goto dark;

		if (i2c_send_byte(0x07) ||
		    i2c_send_byte(buf[0]))
			goto dark;

		i2c_stop(0);

		if (i2c_start(ADDRESS, 0))
			goto dark;

		if (i2c_send_byte(0x08) ||
		    i2c_send_byte(0xA6))
			goto bright;

		i2c_stop(0);

		if (i2c_start(ADDRESS, 0))
			goto dark;

		if (i2c_send_byte(0x08))
			goto bright;

		i2c_stop(0);

		if (i2c_start(ADDRESS, 1))
			goto dark;

		i2c_receive(buf, 1);

		i2c_stop(1);
		pin_low(LED1);
#endif
	}

dark:
	pin_low(LED1);
bright:
	while (1) {
		pin_toggle(LED1);
		delay(0x1000);
		pin_toggle(LED1);
		delay(0xFFFF);
	}
}
