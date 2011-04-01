.PHONY: all examples clean

all: examples

examples:
	$(MAKE) -C examples all

clean:
	$(MAKE) -C examples clean
