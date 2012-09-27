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
#include <watchdog.h>
#include <clock.h>
#include <pins.h>
#include <usi.h>

#define CE   1.0
#define IRQ  1.3

#define CSN  1.4
#define SCLK 1.5
#define SDO  1.6
#define SDI  1.7

#include "lib/nRF24L01P.c"

void
port1_interrupt(void)
{
	pin_interrupt_disable(IRQ);
	pin_interrupt_clear(IRQ);
	LPM0_EXIT;
}

#include "lib/serial.c"

static const unsigned char addr[] = { 0xB1, 0xB2, 0xB3, 0xB4, 0xB5 };

int
main(void)
{
	watchdog_off();
	clock_init_1MHz();

	/* set all pins but SDI and IRQ in output mode */
	port1_direction = ~(pin_mask(IRQ) | pin_mask(SDI));

	/* set all pins low except IRQ and CSN */
	port1_output = pin_mask(IRQ) | pin_mask(CSN);

	/* configure IRQ/button */
	pin_resistor_enable(IRQ);
	pin_interrupt_falling(IRQ);

	serial_init();

	/* initialize SPI */
	usi_spi_master_init();
	usi_spi_clock_d1();

	/* enable interrupts */
	__eint();

	/* flush buffers and clear interrupts */
	nrf_rx_flush();
	nrf_tx_flush();
	nrf_set_status(NRF_RX_DR | NRF_TX_DS | NRF_MAX_RT);
	nrf_sleep();

	/* wait for keypress */
	pin_interrupt_clear(IRQ);
	pin_interrupt_enable(IRQ);
	LPM0;

	serial_puts("Initializing.. ");
	/* enable dynamic payload */
	nrf_set_feature(NRF_EN_DPL);

	/* enable P0 */
	nrf_set_enable(NRF_ERX_P0);
	/* enable auto acknowledge on P0 */
	nrf_set_aa(NRF_ENAA_P0);
	/* enable dynamic payload on P0 */
	nrf_set_dynpd(NRF_DPL_P0);
	/* set address for P0 */
	nrf_rx_addr_p0(addr);

	nrf_prx_wakeup();

	pin_high(CE);
	serial_puts("done\n");

	while (1) {
		unsigned char status;

		/* wait for interrupt */
		__dint();
		if (pin_is_high(IRQ)) {
			pin_interrupt_enable(IRQ);
			_BIS_SR(LPM0_bits | GIE);
		} else
			__eint();

		status = nrf_status();
		if (status & NRF_RX_DR) {
			unsigned char len;
			char buf[33];

			nrf_set_status(NRF_RX_DR);

			len = nrf_payload_width();
			if (len <= 32) {
				nrf_get_payload((unsigned char *)buf, len);
				buf[len] = '\0';
				serial_puts(buf);
			} else {
				nrf_rx_flush();
				serial_puts("Discarded packet\n");
			}

			continue;
		}

		serial_puts("Hmm.. spurious wakeup\n");
	}
}
