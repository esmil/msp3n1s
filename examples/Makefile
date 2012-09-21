examples = \
	blink pwm 74hc595 74hc166 rotenc ds1820 serial \
	nRF24L01P oneslave lcd lcdslave

all:

%:
	@for i in $(examples); do\
	  $(MAKE) -C $$i $@;\
	done
