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

#include "lib/serial_common.c"

#ifndef SERIAL_TX_BUFSIZE
#define SERIAL_TX_BUFSIZE 16
#endif

static struct {
	unsigned char buf[SERIAL_TX_BUFSIZE];
	volatile unsigned char start;
	volatile unsigned char end;
} serial_output;

void
timera_cc0_interrupt(void)
{
	static unsigned int txbyte;

	if (txbyte == 0) {
		unsigned char start = serial_output.start;

		if (start == serial_output.end) {
			timera_cc0_interrupt_disable();
			return;
		} else {
			txbyte = (serial_output.buf[start] | 0x100) << 1;
			serial_output.start = (start + 1) % SERIAL_TX_BUFSIZE;
		}
	}

	TACCTL0 |= OUTMOD2;          /* clear TX on next interrupt */
	if (txbyte & 0x01)
		TACCTL0 &= ~OUTMOD2; /* set TX on next interrupt */

	timera_cc0 += BITTIME;       /* add offset */

	txbyte >>= 1;
}

static void
serial__transmit(void)
{
	if (timera_cc0_interrupt_enabled())
		return;

	timera_cc0 = timera_count;
	timera_cc0_interrupt_raise();
	timera_cc0_interrupt_enable();
}

static void
serial__char(unsigned char c)
{
	unsigned char end = serial_output.end;

	serial_output.buf[end] = c;
	serial_output.end = (end + 1) % SERIAL_TX_BUFSIZE;
}

static void __attribute__((unused))
serial_putchar(unsigned char c)
{
	serial__char(c);
	serial__transmit();
}

static void __attribute__((unused))
serial__string(const char *p)
{
	unsigned char c;

	for (c = *p++; c != '\0'; c = *p++)
		serial__char(c);
}

static void __attribute__((unused))
serial_puts(const char *p)
{
	serial__string(p);
	serial__transmit();
}

static void __attribute__((unused))
serial__uint_b10(unsigned int n)
{
	if (n >= 10000)
		serial__char('0' + (n / 10000));

	if (n >= 1000)
		serial__char('0' + (n / 1000) % 10);

	if (n >= 100)
		serial__char('0' + (n / 100) % 10);

	if (n >= 10)
		serial__char('0' + (n / 10) % 10);

	serial__char('0' + n % 10);
}

static const unsigned char serial_hexdigits[] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static void __attribute__((unused))
serial__uint_b16(unsigned int n)
{
	if (n > 0xFFF)
		serial__char(serial_hexdigits[n >> 12]);

	if (n > 0xFF)
		serial__char(serial_hexdigits[(n >> 8) & 0xF]);

	if (n > 0xF)
		serial__char(serial_hexdigits[(n >> 4) & 0xF]);

	serial__char(serial_hexdigits[n & 0xF]);
}

static void __attribute__((unused))
serial_dump(const void *buf, unsigned int len)
{
	const unsigned char *p = buf;

	for (; len > 0; len--) {
		int c = *p++;

		serial__char(serial_hexdigits[c >> 4]);
		serial__char(serial_hexdigits[c & 0xF]);
	}

	serial__char('\n');
	serial__transmit();
}

static void __attribute__((unused))
serial__int_b10(int n)
{
	if (n < 0) {
		serial__char('-');
		n = -n;
	}

	serial__uint_b10(n);
}

static void __attribute__((unused))
serial_printf(const char *fmt, ...)
{
	unsigned char c;
	va_list ap;

	va_start(ap, fmt);

	for (c = *fmt++; c != '\0'; c = *fmt++) {
		if (c != '%') {
			serial__char(c);
			continue;
		}

		switch (*fmt++) {
		case '%':
			serial__char('%');
			break;
#ifdef SERIAL_PRINTF_U
		case 'u':
			serial__uint_b10(va_arg(ap, unsigned int));
			break;
#endif
#ifdef SERIAL_PRINTF_X
		case 'x':
			serial__uint_b16(va_arg(ap, unsigned int));
			break;
#endif
#ifdef SERIAL_PRINTF_D
		case 'd':
			serial__int_b10(va_arg(ap, int));
			break;
#endif
#ifdef SERIAL_PRINTF_C
		case 'c':
			serial__char(va_arg(ap, unsigned int));
			break;
#endif
#ifdef SERIAL_PRINTF_S
		case 's':
			serial__string(va_arg(ap, const char *));
			break;
#endif
		default:
			goto out;
		}
	}
out:
	va_end(ap);

	serial__transmit();
}

static void
serial_init_tx(void)
{
	pin_mode_output(SERIAL_TX);
	pin_function_primary(SERIAL_TX);
	/* set TX high when idle */
	timera_cc0_config(TIMERA_CC_OUTPUT_HIGH | TIMERA_CC_OUTPUT_SET);
}
