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
#include <delay.h>

#ifndef ONEWIRE_PIN
#error "ONEWIRE_PIN not defined"
#endif

static unsigned char __attribute__((unused))
onewire_crc(const unsigned char *data, unsigned int len)
{
	unsigned char crc = 0;

	for (; len; len--) {
		unsigned char c = *data++;
		unsigned int i;

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
#ifdef ONEWIRE_INTERNAL_PULLUP
	pin_low(ONEWIRE_PIN);
#endif
	pin_mode_output(ONEWIRE_PIN);
}

static inline void
onewire_pin_release(void)
{
	pin_mode_input(ONEWIRE_PIN);
#ifdef ONEWIRE_INTERNAL_PULLUP
	pin_high(ONEWIRE_PIN);
#endif
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
onewire_transmit_1bit(unsigned char bit)
{
	if (bit) {
		onewire_pin_low();
		onewire_pin_release();
		delay_cycles(55);
	} else {
		onewire_pin_low();
		delay_cycles(60);
		onewire_pin_release();
		delay_cycles(5);
	}
}

static void
onewire_transmit_8bit(unsigned char byte)
{
	unsigned char mask;

	for (mask = 1; mask; mask <<= 1)
		onewire_transmit_1bit(byte & mask);
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
		if (onewire_receive_1bit())
			byte |= mask;
	}

	return byte;
}

static int __attribute__((unused))
onewire_read_rom(unsigned char rom[8])
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
onewire_match_rom(const unsigned char rom[8])
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
onewire_skip_rom(void)
{
	if (onewire_reset())
		return -1;

	/* transmit SKIP ROM command */
	onewire_transmit_8bit(0xCC);

	return 0;
}

static int
onewire__search(unsigned char *state, unsigned char rom[8])
{
	unsigned char lastzb = 0;
	unsigned char bit = 1;
	unsigned int i;

	for (i = 0; i < 8; i++) {
		unsigned char mask;

		for (mask = 1; mask; mask <<= 1) {
			unsigned char tx;

			if (bit > *state)
				rom[i] &= ~mask;

			if (onewire_receive_1bit()) {
				if (onewire_receive_1bit()) {
					/* no slaves */
					return -1;
				} else {
					/* only slaves with 1 bits here */
					rom[i] |= mask;
					tx = 1;
				}
			} else {
				if (onewire_receive_1bit()) {
					/* only slaves with 0 bits here */
					tx = 0;
				} else {
					/* branch point */
					if (bit == *state) {
						rom[i] |= mask;
						tx = 1;
					} else {
						tx = rom[i] & mask;
						if (tx == 0)
							lastzb = bit;
					}
				}
			}

			onewire_transmit_1bit(tx);
			bit++;
		}
	}

	*state = lastzb;
	return 0;
}

static int __attribute__((unused))
onewire_search_rom(unsigned char *state, unsigned char rom[8])
{
	if (onewire_reset())
		return -1;

	/* transmit SEARCH ROM command */
	onewire_transmit_8bit(0xF0);

	return onewire__search(state, rom);
}

static int __attribute__((unused))
onewire_alarm_search(unsigned char *state, unsigned char rom[8])
{
	if (onewire_reset())
		return -1;

	/* transmit ALARM SEARCH command */
	onewire_transmit_8bit(0xEC);

	return onewire__search(state, rom);
}
