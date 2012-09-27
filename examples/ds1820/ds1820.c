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
#include <delay.h>

#define LED1 1.0
#define LED2 1.6
#define S2   1.3

#define SERIAL_PRINTF_D
#include "lib/serial_tx.c"

#define ONEWIRE_PIN 1.5
/* #define ONEWIRE_INTERNAL_PULLUP */
#include "lib/onewire.c"

#define DTEMP_PARASITE_POWER

static void
dtemp__convert(void)
{
	/* transmit convert temperature command */
	onewire_transmit_8bit(0x44);

#ifdef DTEMP_PARASITE_POWER
#ifdef ONEWIRE_INTERNAL_PULLUP
/* The internal pull-up of the MSP430G2231 is around 35kOhm, but
 * 1-Wire specifies a pull-up of 4.7kOhm, yet the internal pull-up
 * seems to be sufficient for up to 2 or 3 DS1820s.
 * However when the DS1820 is configured in parasite power mode
 * the internal pull-up doesn't seem to work.
 */
#error "The internal pull-up doesn't work with DS1820 parasite power mode"
#endif
	/* enable "strong pull-up" for at least 750ms */
	pin_high(ONEWIRE_PIN);
	pin_mode_output(ONEWIRE_PIN);
	for (unsigned int i = 0; i < 750; i++)
		delay_cycles(1000);
	pin_mode_input(ONEWIRE_PIN);
	pin_low(ONEWIRE_PIN);
#else
	/* wait for conversion to complete */
	while (onewire_receive_1bit() == 0);
#endif
}

static int
dtemp__scratchpad_read(unsigned char scratchpad[9])
{
	unsigned int i;

	/* transmit read scratchpad command */
	onewire_transmit_8bit(0xBE);

	/* read scratchpad bytes */
	for (i = 0; i < 9; i++)
		scratchpad[i] = onewire_receive_8bit();

	return onewire_crc(scratchpad, 9);
}

static int __attribute__((unused))
dtemp_convert(unsigned char rom[8], unsigned char scratchpad[9])
{
	if (onewire_match_rom(rom))
		return -1;

	dtemp__convert();

	if (onewire_match_rom(rom))
		return -1;

	return dtemp__scratchpad_read(scratchpad);
}

static int __attribute__((unused))
dtemp_convert_single(unsigned char scratchpad[9])
{
	if (onewire_skip_rom())
		return -1;

	dtemp__convert();

	if (onewire_skip_rom())
		return -1;

	return dtemp__scratchpad_read(scratchpad);
}

void
port1_interrupt(void)
{
	pin_interrupt_disable(S2);
	LPM0_EXIT;
}

static void
read_temperature(unsigned char rom[8])
{
	unsigned char scratchpad[9];
	int val;

	pin_high(LED1);
	val = dtemp_convert(rom, scratchpad);
	pin_low(LED1);

	if (val) {
		serial_printf("Error reading temperature\n");
		return;
	}

	val = (scratchpad[1] << 8) | scratchpad[0];

	serial_printf("Read %d.%d C\n", val / 2, (val & 1) ? 5 : 0);
	serial_printf("..or %d - 0.25 + %d/%d C\n", val / 2,
			(int)(scratchpad[7] - scratchpad[6]),
			(int)scratchpad[7]);

	/*
	serial_printf("Scratchpad: ");
	serial_dump(scratchpad, 9);
	*/
}

int
main(void)
{
	watchdog_off();
	clock_init_1MHz();

	/* set all pins to output high */
	port1_direction = 0xFF;
	port1_output = 0xFF;

	pin_low(LED1);

	/* initialize S2 to receive button press interrupt */
	pin_mode_input(S2);
	pin_resistor_enable(S2);
	pin_high(S2);
	pin_interrupt_falling(S2);

	/* initialize onewire */
	onewire_init();

	/* initialize serial output */
	serial_init_clock();
	serial_init_tx();

	/* enable interrupts */
	__eint();

	while (1) {
		unsigned char search_state = 0;
		unsigned char rom[8];

		/* wait for button press */
		pin_low(LED2);
		pin_interrupt_clear(S2);
		pin_interrupt_enable(S2);
		LPM0;
		pin_high(LED2);

		do {
			if (onewire_search_rom(&search_state, rom)) {
				serial_printf("Error during search\n");
				break;
			}

			if (onewire_crc(rom, 8)) {
				serial_printf("Error validating ROM\n");
				break;
			}

			serial_printf("\nFound slave: ");
			serial_dump(rom, 8);

			/* if this slave is in the DS1820 family
			 * read the temperature */
			if (rom[0] == 0x10)
				read_temperature(rom);

		} while (search_state);

		serial_printf("Done.\n");
	}
}
