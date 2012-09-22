msp3n1s
=======


About
-----

This is my collection of code for the [MSP430 Launchpad][lp].

[lp]: http://processors.wiki.ti.com/index.php/MSP430_LaunchPad_(MSP-EXP430G2)


Usage
-----

You'll need to have msp430-gcc and the rest of the MSP430 GCC toolchain installed
along with mspdebug. Once that is taken care check out the code like this

    $ git clone git://github.com/esmil/msp3n1s.git
    
Now enter one of the examples directories

    $ cd msp3n1s/examples/blink

and do

    $ make upload

to build and write the program to your Launchpad. Running `make` will only build the program.


Headers
-------

The `headers` subdirectory contains a collection of headers with inline functions,
which are basically readable names for what you can do with peripheral registers.
Compare
```C
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;
	P1DIR |= BIT0;

	while (1) {
		P1OUT |= BIT0;
		delay(0xFFFF);
		P1OUT &= ~BIT0;
		delay(0xFFFF);
		}
}
```
to this program
```C
int main(void)
{
	watchdog_off();
	pin_mode_output(1.0);

	while (1) {
		pin_high(1.0);
		delay(0xFFFF);
		pin_low(1.0);
		delay(0xFFFF);
	}
}
```

I find that the second program is much easier to read and understand,
and since GCC is clever there is absolutely no overhead in doing this.
In fact the two programs compile to the exact same thing.


Libraries
---------

The `lib` subdirectory contains a bunch of code which is meant to be shared among projects.
Since programs for the Launchpad tend to be rather small, most of this code is
meant to be included directly into your source code using

    #include "lib/<library>.c"

This allows the compiler to better optimize the whole program,
and throw away unused pieces of code.

All libraries should have at least one program in the `examples` subdirectory
using them.

So far these libraries include

* onewire.c

  Dallas 1-wire master code.

* lcd.c

  Code to control Hitachi HD44780-based LCD displays.

* serial.c

  Software implementation of RS232 serial communication for chips without a UART.

License
-------

msp3n1s is free software. It is distributed under the terms
of the [GNU General Public License][gpl].

[gpl]: http://www.fsf.org/licensing/licenses/gpl.html


Contact
-------

Please send bug reports, patches, feature requests, praise and general gossip
to me, Emil Renner Berthing <esmil@mailme.dk>.
