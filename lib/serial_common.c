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

#ifndef _SERIAL_COMMON_C
#define _SERIAL_COMMON_C

#include <timera.h>
#include <stdarg.h>

#define SERIAL_TX 1.1
#define SERIAL_RX 1.2

#ifndef BITTIME
/* conditions for 1Mhz / (13*8) ~= 9600 bps software UART */
#define BITTIME   (13*8)
#endif

static void
serial_init_clock(void)
{
	timera_clock_source_smclk();
	timera_clock_divide(1);
	timera_mode_continuous();
}

#endif
