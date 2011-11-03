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
#include <timera.h>
#include <delay.h>

static unsigned char __attribute__((unused))
onewire_crc_8bit(const unsigned char *message, int len)
{

	unsigned char crc = 0;
	unsigned char c;

	for (c = *message++; len; c = *message++, len--) {
		int i;

		for (i = 0; i < 8; i++) {
			if ((crc ^ c) & 1)
				crc = (crc >> 1) ^ 0x8C;
			else
				crc >>= 1;

			c >>= 1;
		}
	}

	return crc;
}

static inline void
onewire_pin_low(void)
{
	pin_mode_output(ONEWIRE_PIN);
#ifdef ONEWIRE_INTERNAL_PULLUP
	pin_low(ONEWIRE_PIN);
#endif
}

static inline void
onewire_pin_release(void)
{
#ifdef ONEWIRE_INTERNAL_PULLUP
	pin_high(ONEWIRE_PIN); /* enable internal pull-up */
#endif
	pin_mode_input(ONEWIRE_PIN);
}

static void
onewire_init(void)
{
#ifdef ONEWIRE_INTERNAL_PULLUP
	pin_resistor_enable(ONEWIRE_PIN);
	onewire_pin_release();
#else
	onewire_pin_release();
	pin_low(ONEWIRE_PIN);
#endif
}

static int
onewire_reset(void)
{
	onewire_pin_low();

	/* sleep for at least 480 us */
	delay_cycles(500);

	onewire_pin_release();

	delay_cycles(65);

	if (pin_is_high(ONEWIRE_PIN))
		return -1;

	delay_cycles(435);

	return 0;
}

static void
onewire_transmit_8bit(unsigned char byte)
{
	unsigned char mask;

#ifndef ONEWIRE_INTERNAL_PULLUP
	pin_high(ONEWIRE_PIN);
#endif
	pin_mode_output(ONEWIRE_PIN);

	for (mask = 1; mask; mask <<= 1) {
		if (byte & mask) {
			pin_low(ONEWIRE_PIN);
			pin_high(ONEWIRE_PIN);
			delay_cycles(55);
		} else {
			pin_low(ONEWIRE_PIN);
			delay_cycles(65);
			pin_high(ONEWIRE_PIN);
			delay_cycles(5);
		}
	}

	pin_mode_input(ONEWIRE_PIN);
#ifndef ONEWIRE_INTERNAL_PULLUP
	pin_low(ONEWIRE_PIN);
#endif
}

static unsigned char
onewire_receive_1bit(void)
{
	unsigned char ret;

	onewire_pin_low();
	onewire_pin_release();

	delay_cycles(9);

	ret = pin_is_high(ONEWIRE_PIN);

	delay_cycles(53);

	return ret;
}

static unsigned char
onewire_receive_8bit(void)
{
	unsigned char byte = 0;
	unsigned char mask;

	for (mask = 1; mask; mask <<= 1) {
		onewire_pin_low();
		onewire_pin_release();

		delay_cycles(9);

		if (pin_is_high(ONEWIRE_PIN))
			byte |= mask;

		delay_cycles(53);
	}

	return byte;
}

static int __attribute__((unused))
onewire_rom_read(unsigned char rom[8])
{
	unsigned int i;

	if (onewire_reset())
		return -1;

	/* transmit READ ROM command */
	onewire_transmit_8bit(0x33);

	for (i = 0; i < 8; i++)
		rom[i] = onewire_receive_8bit();

	return 0;
}

static int __attribute__((unused))
onewire_rom_match(const unsigned char rom[8])
{
	unsigned int i;

	if (onewire_reset())
		return -1;

	/* transmit MATCH ROM command */
	onewire_transmit_8bit(0x55);

	for (i = 0; i < 8; i++)
		onewire_transmit_8bit(rom[i]);

	return 0;
}

static int __attribute__((unused))
onewire_rom_skip(void)
{
	if (onewire_reset())
		return -1;

	/* transmit SKIP ROM command */
	onewire_transmit_8bit(0xCC);

	return 0;
}
