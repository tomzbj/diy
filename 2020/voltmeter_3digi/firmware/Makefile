# hardware
FLASH_SIZE := 8K
MEM_SIZE := 1K

# toolchain
MCU		:= STM8S103
STVP_CMDLINE = "d:/EDA/stvp/stvp/stvp_cmdline.exe"
CXDIR 	:= d:/EDA/COSMIC/CXSTM8
DINCDIR := $(patsubst %,-i%, $(CXDIR)/Hstm8 $(CXDIR)/stm8s/inc)
STDLIBDIR := $(CXDIR)/stm8s/src
DLIBDIR := $(CXDIR)/Lib 
CFLAGS  := +split +compact -pxp -pp -l
DDEFS	:= -d $(MCU) -d F_CPU=16000000 -d USE_STDPERIPH_DRIVER
LKF		:= _lkf/stm8.lkf
PROJECT	:= main
#DINCDIR += $(patsubst %,-i./hw/%, usart eeprom fm morse)

SRC 	:= $(wildcard *.c _libs/*.c app/*.c drv/*.c bsp/*.c thirdparty/*/*.c) # */

DINCDIR += $(patsubst %,-i./%, _libs app drv bsp)
DINCDIR += $(patsubst %,-i./thirdparty/%, xprintf ztask zss)

DINCDIR += $(UINCS)

SRC += $(STDLIBDIR)/stm8s_gpio.c
SRC += $(STDLIBDIR)/stm8s_clk.c
SRC += $(STDLIBDIR)/stm8s_adc1.c
SRC += $(STDLIBDIR)/stm8s_flash.c
#SRC += $(STDLIBDIR)/stm8s_uart1.c
#SRC += $(STDLIBDIR)/stm8s_tim2.c
SRC += $(STDLIBDIR)/stm8s_tim4.c
#SRC += $(STDLIBDIR)/stm8s_exti.c

RM	:= @rm -f

OBJS	:= $(SRC:.c=.o)

# rules 
all: $(OBJS) $(PROJECT).elf $(PROJECT).hex
	@size $(PROJECT).elf
	@arr=(`size $(PROJECT).elf | sed -n '2p'`); \
	let flash=($${arr[0]}+$${arr[1]}); \
	let mem=($${arr[1]}+$${arr[2]}); \
	let flash_size=$(subst K,,$(FLASH_SIZE))*1024; \
	let mem_size=$(subst K,,$(MEM_SIZE))*1024; \
	flash_usage=`echo "scale=1;($$flash*100/$$flash_size)" | bc`; \
	mem_usage=`echo "scale=1;($$mem*100/$$mem_size)" | bc`; \
	echo "Flash: $$flash / $$flash_size bytes, $$flash_usage% Full (.text + .data)"; \
	echo "SRAM:  $$mem / $$mem_size bytes, $$mem_usage% Full (.data + .bss)"; 

%.o: %.c
#	cxstm8 +mods0 -libf $(CFLAGS) $(DDEFS) $(DINCDIR) -cl. -co. $<
	@cxstm8 +mods0 $(CFLAGS) $(DDEFS) $(DINCDIR) -cl. -co. $< > /dev/nul

%.elf: $(OBJS) $(LKF)
	clnk -m $(PROJECT).map -l $(DLIBDIR) -o $(PROJECT).sm8 $(LKF)
	cvdwarf $(PROJECT).sm8

%.hex: %.elf
	chex -fi -o $(PROJECT).hex $(PROJECT).sm8

%.lkf: %.lkf.template 
	sed -e "s/%%OBJS%%/$(subst stm8_interrupt_vector.o,,$(notdir $(OBJS)))/" $< > $@

flash:
	$(STVP_CMDLINE) -device=$(MCU)F3 -fileprog=main.hex -verif -no_loop | grep "<<<"

af: all flash

clean:
	$(RM) *.o *.elf *.ls *.hex *.sm8 *.map *~ *.*~ $(LKF)

.PHONY: clean all flash
.SECONDARY: %.lkf
