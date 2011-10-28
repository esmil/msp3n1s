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

#ifndef _NRF24L01P_H
#define _NRF24L01P_H

/* SPI commands */
#define NRF_R_REGISTER         0x00
#define NRF_W_REGISTER         0x20
#define NRF_R_RX_PAYLOAD       0x61
#define NRF_W_TX_PAYLOAD       0xA0
#define NRF_FLUSH_TX           0xE1
#define NRF_FLUSH_RX           0xE2
#define NRF_REUSE_TX_PL        0xE3
#define NRF_R_RX_PL_WID        0x60
#define NRF_W_ACK_PAYLOAD      0xA8
#define NRF_NOP                0xFF

/* registers */
#define NRF_CONFIG             0x00
#define NRF_EN_AA              0x01
#define NRF_EN_RXADDR          0x02
#define NRF_SETUP_AW           0x03
#define NRF_SETUP_RETR         0x04
#define NRF_RF_CH              0x05
#define NRF_RF_SETUP           0x06
#define NRF_STATUS             0x07
#define NRF_OBSERVE_TX         0x08
#define NRF_RPD                0x09
#define NRF_RX_ADDR_P0         0x0A
#define NRF_RX_ADDR_P1         0x0B
#define NRF_RX_ADDR_P2         0x0C
#define NRF_RX_ADDR_P3         0x0D
#define NRF_RX_ADDR_P4         0x0E
#define NRF_RX_ADDR_P5         0x0F
#define NRF_RX_ADDR_P(n) (NRF_RX_ADDR_P0 + (n))
#define NRF_TX_ADDR            0x10
#define NRF_RX_PW_P0           0x11
#define NRF_RX_PW_P1           0x12
#define NRF_RX_PW_P2           0x13
#define NRF_RX_PW_P3           0x14
#define NRF_RX_PW_P4           0x15
#define NRF_RX_PW_P5           0x16
#define NRF_RX_PW_P(n) (NRF_RX_PW_P0 + (n))
#define NRF_FIFO_STATUS        0x17
#define NRF_DYNPD              0x1C
#define NRF_FEATURE            0x1D

/* flags */
#define NRF_MASK_RX_DR         0x40
#define NRF_MASK_TX_DS         0x20
#define NRF_MASK_MAX_RT        0x10
#define NRF_EN_CRC             0x08
#define NRF_CRCO               0x04
#define NRF_PWR_UP             0x02
#define NRF_PRIM_RX            0x01

#define NRF_ENAA_P5            0x20
#define NRF_ENAA_P4            0x10
#define NRF_ENAA_P3            0x08
#define NRF_ENAA_P2            0x04
#define NRF_ENAA_P1            0x02
#define NRF_ENAA_P0            0x01
#define NRF_ENAA_P(n) (1 << (n))

#define NRF_ERX_P5            0x20
#define NRF_ERX_P4            0x10
#define NRF_ERX_P3            0x08
#define NRF_ERX_P2            0x04
#define NRF_ERX_P1            0x02
#define NRF_ERX_P0            0x01
#define NRF_ERX_P(n) (1 << (n))

#define NRF_AW_3BYTES          0x01
#define NRF_AW_4BYTES          0x02
#define NRF_AW_5BYTES          0x03

#define NRF_ARD_250US          0x00
#define NRF_ARD_500US          0x10
#define NRF_ARD_750US          0x20
#define NRF_ARD_1000US         0x30
#define NRF_ARD_1250US         0x40
#define NRF_ARD_1500US         0x50
#define NRF_ARD_1750US         0x60
#define NRF_ARD_2000US         0x70
#define NRF_ARD_2250US         0x80
#define NRF_ARD_2500US         0x90
#define NRF_ARD_2750US         0xA0
#define NRF_ARD_3000US         0xB0
#define NRF_ARD_3250US         0xC0
#define NRF_ARD_3500US         0xD0
#define NRF_ARD_3750US         0xE0
#define NRF_ARD_4000US         0xF0

#define NRF_CONF_WAVE          0x80
#define NRF_DR_250KBPS         0x10
#define NRF_DR_1MBPS           0x00
#define NRF_DR_2MBPS           0x04
#define NRF_TX_PWR_18DB        0x00
#define NRF_TX_PWR_12DB        0x02
#define NRF_TX_PWR_6DB         0x04
#define NRF_TX_PWR_0DB         0x06

#define NRF_RX_DR              0x40
#define NRF_TX_DS              0x20
#define NRF_MAX_RT             0x10
#define NRF_RX_P_NO_MASX       0x0E
#define NRF_TX_FULL            0x01

#define NRF_PLOS_CNT_MASK      0xF0
#define NRF_ARC_CNT_MASK       0x0F

#define NRF_FIFO_TX_REUSE      0x40
#define NRF_FIFO_TX_FULL       0x20
#define NRF_FIFO_TX_EMPTY      0x10
#define NRF_FIFO_RX_FULL       0x02
#define NRF_FIFO_RX_EMPTY      0x01

#define NRF_DPL_P5             0x20
#define NRF_DPL_P4             0x10
#define NRF_DPL_P3             0x08
#define NRF_DPL_P2             0x04
#define NRF_DPL_P1             0x02
#define NRF_DPL_P0             0x01
#define NRF_DPL_P(n) (1 << (n))

#define NRF_EN_DPL             0x04
#define NRF_ACK_PAY            0x02
#define NRF_DYN_ACK            0x01

#endif
