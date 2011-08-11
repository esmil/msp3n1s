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

#define LED1 1.0

#define NOT_READY 0xFFFF

#include "lib/i2c_slave.c"

union word {
	unsigned int word;
	struct {
		unsigned char low;
		unsigned char high;
	} byte;
};

static volatile union word value = { .word = NOT_READY };

static enum i2c_return
receive(unsigned char byte)
{
	value.word = byte;
	return I2C_LAST;
}

static unsigned char transmit_high(void);
static unsigned char transmit_low(void);

static unsigned char
transmit_high(void)
{
	unsigned char ret = value.byte.high;

	if (ret != 0xFF)
		i2c_transmit_set(transmit_low);

	return ret;
}

static unsigned char
transmit_low(void)
{
	unsigned char ret = value.byte.low;

	i2c_transmit_set(transmit_high);
	value.word = NOT_READY;

	return ret;
}

int
main(void)
{
	watchdog_off();
	clock_init_1MHz();

	port1_direction = 0xFF;
	port1_output = 0xFF - pin_mask(LED1);

	i2c_slave_init(42, NULL, receive, transmit_high);

	/* enable interrupts */
	__eint();

	while (1)
		LPM0;
}
