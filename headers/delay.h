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

#ifndef _DELAY_H
#define _DELAY_H

/* delay routine from mspgcc help file
 * this takes 3*n + 1 cycles */
static inline void
delay(register unsigned int n)
{
	__asm__ __volatile__ (
		"1: \n"
		" dec %[n] \n"
		" jne 1b \n"
		: [n] "+r"(n));
}

static inline void
delay_cycles(unsigned int n)
{
	switch (n) {
	case 3: __nop();
	case 2: __nop();
	case 1: __nop();
	case 0: return;
	}

	delay((n-1)/3);

	switch (n % 3) {
	case 0: __nop();
	case 2: __nop();
	}
}

#endif
