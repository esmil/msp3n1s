examples = blink pwm 74hc595 rotenc ds1820 serial nRF24L01P

%:
	@for i in $(examples); do\
	  $(MAKE) -C $$i $@;\
	done
