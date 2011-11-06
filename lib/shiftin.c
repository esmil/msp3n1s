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

#include <pins.h>

#ifndef SHIFTIN_CLOCK
#error "SHIFTIN_CLOCK not defined"
#endif

#ifndef SHIFTIN_DATA
#error "SHIFTIN_DATA not defined"
#endif

static unsigned char __attribute__((unused))
shiftin_byte_msb(void)
{
	unsigned char byte = 0;
	unsigned char mask;

	for (mask = 0x80; mask; mask >>= 1) {
		pin_high(SHIFTIN_CLOCK);
		if (pin_is_high(SHIFTIN_DATA))
			byte |= mask;
		pin_low(SHIFTIN_CLOCK);
	}

	return byte;
}

static unsigned char __attribute__((unused))
shiftin_byte_lsb(void)
{
	unsigned char byte = 0;
	unsigned char mask;

	for (mask = 1; mask; mask <<= 1) {
		pin_high(SHIFTIN_CLOCK);
		if (pin_is_high(SHIFTIN_DATA))
			byte |= mask;
		pin_low(SHIFTIN_CLOCK);
	}

	return byte;
}

static void __attribute__((unused))
shiftin_bytes_msb(unsigned char *buf, unsigned int len)
{
	for (; len; len--)
		*buf++ = shiftin_byte_msb();
}

static void __attribute__((unused))
shiftin_bytes_lsb(unsigned char *buf, unsigned int len)
{
	for (; len; len--)
		*buf++ = shiftin_byte_msb();
}

static void __attribute__((unused))
shiftin_bits_msb(unsigned char *buf, unsigned int bits)
{
	while (1) {
		unsigned char mask;

		for (mask = 0x80; mask; mask >>= 1) {
			pin_high(SHIFTIN_CLOCK);
			if (pin_is_high(SHIFTIN_DATA))
				*buf |= mask;
			else
				*buf &= ~mask;
			pin_low(SHIFTIN_CLOCK);

			bits--;
			if (bits == 0)
				return;
		}

		buf++;
	}
}
