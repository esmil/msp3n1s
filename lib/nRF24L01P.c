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

#include "nRF24L01P.h"

#ifndef __unused
#define __unused __attribute__((unused))
#endif

void
usi_interrupt(void)
{
	usi_counter_interrupt_disable();
	LPM0_EXIT;
}

static void
nrf_wait_byte(void)
{
	usi_counter_set(8);
	usi_counter_interrupt_enable();
	LPM0;
}

static void __unused
nrf_write(unsigned char command, const unsigned char *buf, unsigned int len)
{
	pin_low(CSN);
	usi_output_enable();
	usi_data_low = command;
	nrf_wait_byte();

	for (; len; len--) {
		usi_data_low = *buf++;
		nrf_wait_byte();
	}

	usi_output_disable();
	pin_high(CSN);
}

static void __unused
nrf_read(unsigned char command, unsigned char *buf, unsigned int len)
{
	pin_low(CSN);
	usi_output_enable();
	usi_data_low = command;
	nrf_wait_byte();
	usi_output_disable();

	for (; len; len--) {
		nrf_wait_byte();
		*buf++ = usi_data_low;
	}

	pin_high(CSN);
}

static unsigned char __unused
nrf_status(void)
{
	nrf_read(NRF_NOP, NULL, 0);
	return usi_data_low;
}

static void __unused
nrf_tx_addr(const unsigned char *addr)
{
	nrf_write(NRF_W_REGISTER | NRF_TX_ADDR, addr, 5);
}

static void __unused
nrf_rx_addr_p0(const unsigned char *addr)
{
	nrf_write(NRF_W_REGISTER | NRF_RX_ADDR_P0, addr, 5);
}

static void __unused
nrf_set_payload(const unsigned char *buf, unsigned int len)
{
	nrf_write(NRF_W_TX_PAYLOAD, buf, len);
}

static unsigned char __unused
nrf_payload_width(void)
{
	unsigned char ret;
	nrf_read(NRF_R_RX_PL_WID, &ret, 1);
	return ret;
}

static void __unused
nrf_get_payload(unsigned char *buf, unsigned int len)
{
	nrf_read(NRF_R_RX_PAYLOAD, buf, len);
}

static void __unused
nrf_tx_flush(void)
{
	nrf_read(NRF_FLUSH_TX, NULL, 0);
}

static void __unused
nrf_rx_flush(void)
{
	nrf_read(NRF_FLUSH_RX, NULL, 0);
}

static void __unused
nrf_set_config(unsigned char config)
{
	nrf_write(NRF_W_REGISTER | NRF_CONFIG, &config, 1);
}

static void __unused
nrf_set_aa(unsigned char aa)
{
	nrf_write(NRF_W_REGISTER | NRF_EN_AA, &aa, 1);
}

static void __unused
nrf_set_enable(unsigned char enable)
{
	nrf_write(NRF_W_REGISTER | NRF_EN_RXADDR, &enable, 1);
}

static void __unused
nrf_set_status(unsigned char status)
{
	nrf_write(NRF_W_REGISTER | NRF_STATUS, &status, 1);
}

static void __unused
nrf_set_feature(unsigned char feature)
{
	nrf_write(NRF_W_REGISTER | NRF_FEATURE, &feature, 1);
}

static unsigned char __unused
nrf_fifo_status(void)
{
	unsigned char ret;
	nrf_read(NRF_R_REGISTER | NRF_FIFO_STATUS, &ret, 1);
	return ret;
}

static void __unused
nrf_set_dynpd(unsigned char dynpd)
{
	nrf_write(NRF_W_REGISTER | NRF_DYNPD, &dynpd, 1);
}

static void __unused
nrf_sleep(void)
{
	nrf_set_config(NRF_EN_CRC);
}

static void __unused
nrf_ptx_wakeup(void)
{
	nrf_set_config(NRF_EN_CRC | NRF_PWR_UP);
}

static void __unused
nrf_prx_wakeup(void)
{
	nrf_set_config(NRF_EN_CRC | NRF_PWR_UP | NRF_PRIM_RX);
}
