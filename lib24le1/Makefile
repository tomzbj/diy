CC = sdcc
LD = sdcc
REMOVE = rm -f
PACKIHX = packihx

CFLAGS = -I . -I ./nrf24le1 --std-c99 --opt-code-size --stack-auto

#SRC = $(wildcard *.c nrf24le1/*.c)
SRC = main.c 	nrf24le1_it.c
SRC += nrf24le1/nrf24le1_delay.c
SRC += nrf24le1/nrf24le1_rf.c
SRC += nrf24le1/nrf24le1_gpio.c
SRC += nrf24le1/nrf24le1_uart.c
SRC += nrf24le1/nrf24le1_interrupt.c

FULL_PRJ = main

OBJS = $(SRC:.c=.rel)

all: $(OBJS) $(FULL_PRJ).ihx $(FULL_PRJ).hex size

%.rel: %.c
	$(CC) -c $(CFLAGS) $< 

%.ihx: $(OBJS)
	$(CC) $(notdir $(OBJS))

%.hex: %.ihx
	$(PACKIHX) $(FULL_PRJ).ihx > $(FULL_PRJ).hex 2> /dev/null

size:
	size $(FULL_PRJ).hex

clean:
	rm -f *.rel *.asm *.sym *.lst *.rst *.hex *.ihx *.lk 

.PHONY: clean
