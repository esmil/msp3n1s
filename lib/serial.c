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

#include <signal.h>
#include <timer_a.h>
#include <stdarg.h>

#define TX   1.1
#define RX   1.2

/* conditions for 1Mhz / (13*8) ~= 9600 bps software UART */
#define BITTIME   (13*8)

static volatile unsigned int serial_txbyte;

timer_a_cc0_interrupt()
{
	timer_a_cc0_add(BITTIME);     /* add offset */

	if (serial_txbyte == 0) { /* all bits sent, wake up */
		LPM0_EXIT;
		return;
	}

	TACCTL0 |= OUTMOD2;  /* clear TX on next interrupt */
	if (serial_txbyte & 0x01)
		TACCTL0 &= ~OUTMOD2; /* set TX on next interrupt */

	serial_txbyte >>= 1;
}

static __attribute__((unused)) void
serial__char(unsigned char c)
{
	serial_txbyte = (c | 0x100) << 1;
	LPM0;
}

static __attribute__((unused)) void
serial_putchar(unsigned char c)
{
	timer_a_cc0_set(timer_a_count() + 16);
	timer_a_cc0_interrupt_enable();

	serial__char(c);

	timer_a_cc0_interrupt_disable();
}

static __attribute__((unused)) void
serial__string(const char *p)
{
	unsigned char c;

	for (c = *p++; c != '\0'; c = *p++)
		serial__char(c);
}

static __attribute__((unused)) void
serial_puts(const char *p)
{
	timer_a_cc0_set(timer_a_count() + 16);
	timer_a_cc0_interrupt_enable();

	serial__string(p);

	timer_a_cc0_interrupt_disable();
}

static __attribute__((unused)) void
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

static __attribute__((unused)) void
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

static __attribute__((unused)) void
serial_dump(const void *buf, unsigned int len)
{
	const unsigned char *p = buf;

	timer_a_cc0_set(timer_a_count() + 16);
	timer_a_cc0_interrupt_enable();

	for (; len > 0; len--) {
		int c = *p++;

		serial__char(serial_hexdigits[c >> 4]);
		serial__char(serial_hexdigits[c & 0xF]);
	}

	serial__char('\n');

	timer_a_cc0_interrupt_disable();
}

static __attribute__((unused)) void
serial__int_b10(int n)
{
	if (n < 0) {
		serial__char('-');
		n = -n;
	}

	serial__uint_b10(n);
}

static __attribute__((unused)) void
serial_printf(const char *fmt, ...)
{
	unsigned char c;
	va_list ap;

	va_start(ap, fmt);

	timer_a_cc0_set(timer_a_count() + 16);
	timer_a_cc0_interrupt_enable();

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
	timer_a_cc0_interrupt_disable();
}

static void
serial_init()
{
	pin_mode_output(TX);
	pin_mode_input(RX);
	pin_function_primary(TX);
	pin_function_primary(RX);

	timer_a_cc0_output_high(); /* set TX high when idle */

	timer_a_clock_source_smclk();
	timer_a_clock_divide(1);
	timer_a_cc0_output_mode(1);
	timer_a_mode_continuous();
}

#undef TX
#undef RX
