#ifndef _ONESLAVE_H
#define _ONESLAVE_H

#define ONESLAVE_PIN 1.2

#define ONEWIRE_LAST   (1<<0)
#define ONEWIRE_WAKEUP (1<<1)
#define ONEWIRE_WRITE  (1<<2)

struct onewire_command {
	unsigned char cmd;
	unsigned char flags;
	unsigned int len;
	void *data;
};

extern unsigned char onewire_getcmd(void);

#endif
