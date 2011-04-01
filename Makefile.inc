# Copy this file to your project directory, uncomment
# the two lines below and fill in the name of your project
# and the path to this repository

#NAME =
#BASE =

CC = msp430-gcc
OBJDUMP = msp430-objdump
SIZE = msp430-size
MSPDEBUG = mspdebug
SED = sed
CAT = cat
MODPROBE = modprobe
STTY = stty

MCU = msp430x2231
#MCU = msp430x2211
DRIVER = rf2500
CFLAGS = -O2 -g -mmcu=$(MCU) -Wall -Wextra -pedantic

ifdef BASE
CFLAGS += -iquote$(BASE) -I$(BASE)/headers
endif

# Linux modules needed for serial communication with the chip on the LaunchPad
MODULES = uhci-hcd cdc-acm ti_usb_3410_5052

PORT = /dev/ttyACM0
BAUD = 9600
#BAUD = 2400
MODE_RAW = raw -echo
MODE_7 = cs7
MODE_8 = cs8
MODE_N = -parenb
MODE_E = parenb -parodd
MODE_O = parenb parodd
MODE_1 = -cstopb
MODE_2 = cstopb
MODE = $(MODE_RAW) $(MODE_8) $(MODE_N) $(MODE_1)# 8N1

.PHONY: all modules clean
.PRECIOUS: %.elf

all: $(NAME).elf

%.elf: %.c
	@echo '  CC/LD $@'
	@$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
	@$(SIZE) -A $@ | $(SED) -e '/^\./!d;/^\.debug/d;s/^\([^0-9]*[0-9]*\).*/  \1 bytes/'

%.lss: %.elf
	@echo '  OBJDUMP > $@'
	@$(OBJDUMP) -ld $< | $(SED) -e '/\/.*/d' > $@

list: $(NAME).lss

upload: $(NAME).elf
	@$(MSPDEBUG) $(DRIVER) erase 'load $<'

modules:
	@for i in $(MODULES); do\
	  echo "  MODPROBE $$i";\
	  $(MODPROBE) $$i;\
	done

tty: $(PORT)
	@echo '  STTY -F$(PORT) $(MODE) $(BAUD)'
	@$(STTY) -F$(PORT) $(MODE) $(BAUD)

cat: $(PORT)
	@$(CAT) $(PORT)

clean:
	rm -f *.elf *.lss

# vim: syntax=make
