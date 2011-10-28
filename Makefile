# Copy this file to your project directory, uncomment
# the two lines below and fill in the name of your project
# and the path to this repository

#NAME =
#BASE =

## Set the chip type here
MCU = msp430g2231
#MCU = msp430g2211

## Change this according to your code to make the tty and cat targets work
BAUD = 9600
MODE = $(MODE_RAW) $(MODE_8) $(MODE_N) $(MODE_1)# 8N1

DRIVER = rf2500

CC = msp430-gcc
OBJDUMP = msp430-objdump
SIZE = msp430-size
MSPDEBUG = mspdebug
SED = sed
CAT = cat
MODPROBE = modprobe
STTY = stty

OPT     = 2
CFLAGS  = -mmcu=$(MCU) -O$(OPT) -pipe -g
CFLAGS += -std=gnu99 -Wall -Wextra -Wno-variadic-macros -pedantic
CFLAGS += -fstrict-aliasing -fstrict-overflow
#CFLAGS += -ffunction-sections -fdata-sections

ifdef BASE
CFLAGS += -iquote$(BASE) -I$(BASE)/headers
endif

# Linux modules needed for serial communication with the chip on the LaunchPad
MODULES = uhci-hcd cdc-acm ti_usb_3410_5052

PORT = /dev/ttyACM0
MODE_RAW = raw -echo -hup
MODE_7 = cs7
MODE_8 = cs8
MODE_N = -parenb
MODE_E = parenb -parodd
MODE_O = parenb parodd
MODE_1 = -cstopb
MODE_2 = cstopb

.PHONY: all debug modules clean
.PRECIOUS: %.elf

all: $(NAME).elf

%.elf: %.c
	@echo '  CC/LD $@'
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@$(SIZE) $@ | $(SED) -ne '2s/[\t ]*[0-9]*[\t ]*[0-9]*[\t ]*[0-9]*[\t ]*\([0-9]*\).*/  \1 bytes/p'

%.lst: %.elf
	@echo '  OBJDUMP > $@'
	@$(OBJDUMP) -S $< > $@

%.lss: %.elf
	@echo '  OBJDUMP > $@'
	@$(OBJDUMP) -ld $< | $(SED) -e '/\/.*/d' > $@

list: $(NAME).lst $(NAME).lss

debug:
	$(MSPDEBUG) $(DRIVER)

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
	rm -f *.elf *.lss *.lst

# vim: syntax=make
