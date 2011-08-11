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

#include <stdlib.h>
#include <usi.h>

enum i2c_return {
	I2C_OK,
	I2C_LAST,
	I2C_NACK,
};

static struct {
	unsigned char address;
	void (*start)(void);
	enum i2c_return (*receive)(unsigned char byte);
	unsigned char (*transmit)(void);
} i2c_data;

static inline void
i2c_address_set(unsigned char address)
{
	i2c_data.address = address << 1;
}

static inline void
i2c_start_set(void (*start)(void))
{
	i2c_data.start = start;
}

static inline void
i2c_transmit_set(unsigned char (*transmit)(void))
{
	i2c_data.transmit = transmit;
}

static inline void
i2c_receive_set(enum i2c_return (*receive)(unsigned char byte))
{
	i2c_data.receive = receive;
}

static inline unsigned char
i2c_lastbit(unsigned char c)
{
	return c & 0x01;
}

/*
 * Not supported yet :(
 * #pragma GCC diagnostic push
 */
#pragma GCC diagnostic ignored "-pedantic"
static void
usi_interrupt(void)
{
	static void *state = &&exit;

	if (usi_start_interrupt_flag()) {
		usi_output_disable();
		state = &&check_address;
		usi_counter_set(8);
		usi_start_interrupt_clear();
		return;
	}

	goto *state;

check_address:
	usi_data_low ^= i2c_data.address;
	if (usi_data_low > 1)
		goto release;

	usi_output_enable();
	if (i2c_data.start != NULL)
		i2c_data.start();
	state = usi_data_low ? &&transmit : &&receive;
	usi_data_low = 0x00;
	usi_counter_set(1);
	goto exit;

receive:
	usi_output_disable();
	state = &&receive_confirm;
	usi_counter_set(8);
	goto exit;

receive_confirm:
	usi_output_enable();
	switch (i2c_data.receive(usi_data_low)) {
	case I2C_OK:
		usi_data_low = 0x00;
		state = &&receive;
		break;

	case I2C_LAST:
		usi_data_low = 0x00;
		state = &&release;
		break;

	case I2C_NACK:
		usi_data_low = 0xFF;
		state = &&release;
		break;
	}
	usi_counter_set(1);
	goto exit;

transmit_repeat:
	if (i2c_lastbit(usi_data_low))
		goto release;
transmit:
	usi_output_enable();
	usi_data_low = i2c_data.transmit();
	state = &&transmit_confirm;
	usi_counter_set(8);
	goto exit;

transmit_confirm:
	usi_output_disable();
	state = &&transmit_repeat;
	usi_counter_set(1);
	goto exit;

release:
	usi_output_disable();
	state = &&exit;
exit:
	usi_counter_interrupt_clear();
}
/*
 * Not supported yet :(
 * #pragma GCC diagnostic pop
 */
#pragma GCC diagnostic warning "-pedantic"

static void
i2c_slave_init(unsigned char address,
               void (*start)(void),
               enum i2c_return (*receiver)(unsigned char byte),
	       unsigned char (*transmitter)(void))
{
	P1OUT |= 0xC0;
	P1REN |= 0xC0;
	USICTL0 = USIPE7 | USIPE6 | USISWRST;
	USICTL1 = USII2C | USIIE | USISTTIE;
	USICKCTL = USICKPL;
	USICNT = USIIFGCC;
	usi_release();

	i2c_address_set(address);
	i2c_start_set(start);
	i2c_receive_set(receiver);
	i2c_transmit_set(transmitter);
}
