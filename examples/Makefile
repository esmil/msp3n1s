ex_base_512 = blink pwm rotenc lcd 74hc595
ex_base_1k  = $(ex_base_512) 74hc166 serial oneslave
ex_base_2k  = $(ex_base_1k) ds1820 lcdslave

ex_usi_1k   =
ex_usi_2k   = $(ex_usi_1k) nRF24L01P

ex_usci_1k  =
ex_usci_2k  = $(ex_usci_1k)

ex_temp_1k  =
ex_temp_2k  = $(ex_temp_1k)

ex_adc_1k   =
ex_adc_2k   = $(ex_adc_1k)

ex_comp_1k  =
ex_comp_2k  = $(ex_comp_1k)

ex_msp430g2001 = $(ex_base_512)
ex_msp430g2101 = $(ex_base_1k)
ex_msp430g2201 = $(ex_base_2k)
ex_msp430g2111 = $(ex_base_1k) $(ex_comp_1k)
ex_msp430g2211 = $(ex_base_2k) $(ex_comp_2k)
ex_msp430g2121 = $(ex_base_1k) $(ex_usi_1k)
ex_msp430g2221 = $(ex_base_2k) $(ex_usi_2k)
ex_msp430g2131 = $(ex_base_1k) $(ex_usi_1k) $(ex_temp_1k) $(ex_adc_1k)
ex_msp430g2231 = $(ex_base_2k) $(ex_usi_2k) $(ex_temp_2k) $(ex_adc_2k)

ex_msp430g2102 = $(ex_base_1k) $(ex_usi_1k)
ex_msp430g2202 = $(ex_base_2k) $(ex_usi_2k)
ex_msp430g2302 = $(ex_base_2k) $(ex_usi_2k)
ex_msp430g2402 = $(ex_base_2k) $(ex_usi_2k)
ex_msp430g2402 = $(ex_base_2k) $(ex_usi_2k)
ex_msp430g2112 = $(ex_base_1k) $(ex_usi_1k) $(ex_comp_1k)
ex_msp430g2212 = $(ex_base_2k) $(ex_usi_2k) $(ex_comp_2k)
ex_msp430g2312 = $(ex_base_2k) $(ex_usi_2k) $(ex_comp_2k)
ex_msp430g2412 = $(ex_base_2k) $(ex_usi_2k) $(ex_comp_2k)
ex_msp430g2132 = $(ex_base_1k) $(ex_usi_1k) $(ex_temp_1k) $(ex_adc_1k)
ex_msp430g2232 = $(ex_base_2k) $(ex_usi_2k) $(ex_temp_2k) $(ex_adc_2k)
ex_msp430g2332 = $(ex_base_2k) $(ex_usi_2k) $(ex_temp_2k) $(ex_adc_2k)
ex_msp430g2432 = $(ex_base_2k) $(ex_usi_2k) $(ex_temp_2k) $(ex_adc_2k)
ex_msp430g2152 = $(ex_base_1k) $(ex_usi_1k) $(ex_comp_1k) $(ex_temp_1k) $(ex_adc_1k)
ex_msp430g2252 = $(ex_base_2k) $(ex_usi_2k) $(ex_comp_2k) $(ex_temp_2k) $(ex_adc_2k)
ex_msp430g2352 = $(ex_base_2k) $(ex_usi_2k) $(ex_comp_2k) $(ex_temp_2k) $(ex_adc_2k)
ex_msp430g2452 = $(ex_base_2k) $(ex_usi_2k) $(ex_comp_2k) $(ex_temp_2k) $(ex_adc_2k)

ex_msp430g2203 = $(ex_base_2k) $(ex_usci_2k)
ex_msp430g2303 = $(ex_base_2k) $(ex_usci_2k)
ex_msp430g2403 = $(ex_base_2k) $(ex_usci_2k)
ex_msp430g2213 = $(ex_base_2k) $(ex_usci_2k) $(ex_comp_2k)
ex_msp430g2313 = $(ex_base_2k) $(ex_usci_2k) $(ex_comp_2k)
ex_msp430g2413 = $(ex_base_2k) $(ex_usci_2k) $(ex_comp_2k)
ex_msp430g2513 = $(ex_base_2k) $(ex_usci_2k) $(ex_comp_2k)
ex_msp430g2233 = $(ex_base_2k) $(ex_usci_2k) $(ex_temp_2k) $(ex_adc_2k)
ex_msp430g2333 = $(ex_base_2k) $(ex_usci_2k) $(ex_temp_2k) $(ex_adc_2k)
ex_msp430g2433 = $(ex_base_2k) $(ex_usci_2k) $(ex_temp_2k) $(ex_adc_2k)
ex_msp430g2533 = $(ex_base_2k) $(ex_usci_2k) $(ex_temp_2k) $(ex_adc_2k)
ex_msp430g2153 = $(ex_base_1k) $(ex_usci_1k) $(ex_comp_1k) $(ex_temp_1k) $(ex_adc_1k)
ex_msp430g2253 = $(ex_base_2k) $(ex_usci_2k) $(ex_comp_2k) $(ex_temp_2k) $(ex_adc_2k)
ex_msp430g2353 = $(ex_base_2k) $(ex_usci_2k) $(ex_comp_2k) $(ex_temp_2k) $(ex_adc_2k)
ex_msp430g2453 = $(ex_base_2k) $(ex_usci_2k) $(ex_comp_2k) $(ex_temp_2k) $(ex_adc_2k)
ex_msp430g2553 = $(ex_base_2k) $(ex_usci_2k) $(ex_comp_2k) $(ex_temp_2k) $(ex_adc_2k)

subdirs = $(or $(ex_$(MCU)), $(ex_msp430g2231))

all:

%:
	@for i in $(subdirs); do\
	  $(MAKE) -C $$i $@;\
	done
