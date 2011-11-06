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

#define SHIFTOUT_CLOCK 1.4
#define SHIFTOUT_DATA  1.5
#define LATCH 1.7

#include "lib/shiftout.c"

int
main(void)
{
	watchdog_off();
	clock_init_1MHz();

	/* set all pins in output mode */
	port1_direction = 0xFF;

	/* set LED2, SHIFT_CLOCK, SHIFT_DATA and LATCH low,
	 * and the rest high */
	port1_output = ~(pin_mask(LED2)        |
	                 pin_mask(SHIFTOUT_CLOCK) |
	                 pin_mask(SHIFTOUT_DATA)  |
	                 pin_mask(LATCH));

	/* flash LATCH to make sure we're in sync from now on */
	pin_high(LATCH);
	pin_low(LATCH);

	while (1) {
		/* light LED1 */
		pin_high(LED1);
		/* set Q0 on the 74hc595 */
		shiftout_byte_msb(0x01);
		pin_high(LATCH);
		pin_low(LATCH);
		/* wait a while */
		delay(0xFFFF);

		/* turn off LED1 */
		pin_low(LED1);
		/* set Q7 on the 74hc595 */
		shiftout_byte_msb(0x80);
		pin_high(LATCH);
		pin_low(LATCH);
		/* wait a while */
		delay(0xFFFF);
	}
}
