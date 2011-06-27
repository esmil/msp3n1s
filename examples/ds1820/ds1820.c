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

#include <signal.h>
#include <clock.h>
#include <watchdog.h>
#include <pins.h>

#define LED1 1.0
#define LED2 1.6
#define S2   1.3

#define SERIAL_PRINTF_D
#include "lib/serial.c"

#define ONEWIRE_PIN 1.5
#define ONEWIRE_INTERNAL_PULLUP
#include "lib/onewire.c"

static void
dtemp__convert(void)
{
	/* transmit convert temperature command */
	onewire_transmit_8bit(0x44);

	/* wait for conversion to complete */
	while (onewire_receive_1bit() == 0);
}

static int
dtemp__scratchpad_read(unsigned char scratchpad[9])
{
	int i;

	/* transmit read scratchpad command */
	onewire_transmit_8bit(0xBE);

	/* read scratchpad bytes */
	for (i = 0; i < 9; i++)
		scratchpad[i] = onewire_receive_8bit();

	return onewire_crc_8bit(scratchpad, 9);
}

static int __attribute__((unused))
dtemp_convert(unsigned char rom[8], unsigned char scratchpad[9])
{
	if (onewire_rom_match(rom))
		return -1;

	dtemp__convert();

	if (onewire_rom_match(rom))
		return -1;

	return dtemp__scratchpad_read(scratchpad);
}

static int __attribute__((unused))
dtemp_convert_single(unsigned char scratchpad[9])
{
	if (onewire_rom_skip())
		return -1;

	dtemp__convert();

	if (onewire_rom_skip())
		return -1;

	return dtemp__scratchpad_read(scratchpad);
}

port1_interrupt(void)
{
	pin_interrupt_clear(S2);
	LPM0_EXIT;
}

int
main(void)
{
	watchdog_off();
	clock_init_1MHz();

	pin_mode_output(LED1);
	pin_low(LED1);
	pin_mode_output(LED2);
	pin_low(LED2);

	pin_mode_input(S2);
	pin_resistor_enable(S2);
	pin_high(S2);
	pin_interrupt_falling(S2);
	pin_interrupt_enable(S2);

	onewire_init();
	serial_init();

	eint();

	while (1) {
		unsigned char scratchpad[9];
		int val;

		LPM0;

		pin_high(LED2);
		if (dtemp_convert_single(scratchpad))
			goto error;
		pin_low(LED2);

		val = (scratchpad[1] << 8) | scratchpad[0];

		/*
		serial_dump(rom, 8);
		serial_dump(scratchpad, 9);
		*/

		serial_printf("Read %d.%d C\n", val / 2, (val & 1) ? 5 : 0);
		serial_printf("..or %d - 0.25 + %d/%d C\n", val / 2,
		              (int)(scratchpad[7] - scratchpad[6]),
			      (int)scratchpad[7]);
		continue;

error:
		pin_low(LED1);
		pin_low(LED2);
		serial_puts("No slave found.\n");
	}
}
