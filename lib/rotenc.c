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

#ifndef LEFT
#error "LEFT button not defined"
#endif

#ifndef RIGHT
#error "RIGHT button not defined"
#endif

static volatile enum {
	RS_ER,
	RS_L1,
	RS_L2,
	RS_L3,
	RS_R1,
	RS_R2,
	RS_R3,
	RS_OK,
	RS_XL,
	RS_XR,
	RS_MAX = RS_XL
} rotenc_state = RS_OK;

static const unsigned char rotenc_transition[RS_MAX][4] = {
/*             00     01     10     11  */
/* S_ER */ { RS_ER, RS_ER, RS_ER, RS_OK },
/* S_L1 */ { RS_L2, RS_L1, RS_L3, RS_OK },
/* S_L2 */ { RS_L2, RS_L1, RS_L3, RS_OK },
/* S_L3 */ { RS_L2, RS_ER, RS_L3, RS_XL },
/* S_R1 */ { RS_R2, RS_R3, RS_R1, RS_OK },
/* S_R2 */ { RS_R2, RS_R3, RS_R1, RS_OK },
/* S_R3 */ { RS_R2, RS_R3, RS_ER, RS_XR },
/* S_OK */ { RS_ER, RS_L1, RS_R1, RS_OK }
};

enum rotenc_event {
	ROTENC_NONE  = 0,
	ROTENC_LEFT  = RS_XL - RS_OK,
	ROTENC_RIGHT = RS_XR - RS_OK
};

static enum rotenc_event
rotenc_getevent()
{
	int ret;

	do {
		unsigned int pins = 0;

		if (pin_is_high(LEFT))
			pins |= 1;

		if (pin_is_high(RIGHT))
			pins |= 2;

		rotenc_state = rotenc_transition[rotenc_state][pins];
	} while (rotenc_state < RS_OK);

	ret = rotenc_state - RS_OK;
	rotenc_state = RS_OK;

	return ret;
}
