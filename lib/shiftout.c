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

#ifndef SHIFTOUT_CLOCK
#error "SHIFTOUT_CLOCK not defined"
#endif

#ifndef SHIFTOUT_DATA
#error "SHIFTOUT_DATA not defined"
#endif

static void __attribute__((unused))
shiftout_byte_msb(unsigned char byte)
{
	unsigned char mask;

	for (mask = 0x80; mask; mask >>= 1) {
		if (byte & mask)
			pin_high(SHIFTOUT_DATA);
		else
			pin_low(SHIFTOUT_DATA);
		pin_high(SHIFTOUT_CLOCK);
		pin_low(SHIFTOUT_CLOCK);
	}
}

static void __attribute__((unused))
shiftout_byte_lsb(unsigned char byte)
{
	unsigned char mask;

	for (mask = 1; mask; mask <<= 1) {
		if (byte & mask)
			pin_high(SHIFTOUT_DATA);
		else
			pin_low(SHIFTOUT_DATA);
		pin_high(SHIFTOUT_CLOCK);
		pin_low(SHIFTOUT_CLOCK);
	}
}

static void __attribute__((unused))
shiftout_bytes_msb(const unsigned char *data, unsigned int len)
{
	for (; len; len--)
		shiftout_byte_msb(*data++);
}

static void __attribute__((unused))
shiftout_bytes_lsb(const unsigned char *data, unsigned int len)
{
	for (; len; len--)
		shiftout_byte_lsb(*data++);
}

static void __attribute__((unused))
shiftout_bits_msb(const unsigned char *data, unsigned int bits)
{
	while (1) {
		unsigned char byte = *data++;
		unsigned char mask;

		for (mask = 0x80; mask; mask >>= 1) {
			if (byte & mask)
				pin_high(SHIFTOUT_DATA);
			else
				pin_low(SHIFTOUT_DATA);
			pin_high(SHIFTOUT_CLOCK);
			pin_low(SHIFTOUT_CLOCK);

			bits--;
			if (bits == 0)
				return;
		}
	}
}
