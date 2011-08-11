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

#include <usi.h>

#define SCL 1.6
#define SDA 1.7

static int
i2c_start(unsigned char address, int read)
{
	address <<= 1;
	if (read)
		address |= 1;

	/* set start condition */
	usi_data_low = 0;
	usi_output_and_latch_enable();
	usi_latch_disable();

	usi_data_low = address;
	usi_counter_set(8);
	while (!usi_counter_interrupt_flag());

	usi_output_disable();
	usi_counter_set(1);
	while (!usi_counter_interrupt_flag());

	return usi_data_low & 0x01;
}

static void
i2c_stop(int stop)
{
	usi_output_enable();
	usi_data_low = stop ? 0x00 : 0xFF;
	usi_counter_set(1);
	while (!usi_counter_interrupt_flag());

	if (!stop)
		return;

	usi_data_low = 0xFF;
	usi_latch_enable();
	usi_output_and_latch_disable();
}

static int
i2c_send_byte(unsigned char c)
{
	usi_output_enable();
	usi_data_low = c;
	usi_counter_set(8);
	while (!usi_counter_interrupt_flag());

	usi_output_disable();
	usi_counter_set(1);
	while (!usi_counter_interrupt_flag());

	return usi_data_low & 0x01;
}

static int __attribute__((unused))
i2c_send(const char *str)
{
	unsigned char c;

	for (c = *str++; c; c = *str++) {
		int ret = i2c_send_byte(c);
		if (ret)
			return ret;
	}

	return 0;
}

static unsigned char __attribute__((unused))
i2c_receive_byte(int ack)
{
	unsigned char ret;

	usi_output_disable();
	usi_counter_set(8);
	while (!usi_counter_interrupt_flag());

	usi_output_enable();
	ret = usi_data_low;
	usi_data_low = ack ? 0x00 : 0xFF;
	usi_counter_set(1);
	while (!usi_counter_interrupt_flag());

	return ret;
}

static void __attribute__((unused))
i2c_receive(unsigned char *buf, unsigned int len)
{
	while (len) {
		usi_output_disable();
		usi_counter_set(8);
		while (!usi_counter_interrupt_flag());

		usi_output_enable();
		*buf++ = usi_data_low;
		usi_data_low = --len ? 0x00 : 0xFF;
		usi_counter_set(1);
		while (!usi_counter_interrupt_flag());
	}
}
