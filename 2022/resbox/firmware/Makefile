# hardware
FLASH_SIZE := 32K
MEM_SIZE := 2K

# toolchain
STVP_CMDLINE = "d:/EDA/STVP/stvp/stvp_cmdline.exe"
CXDIR 	:= d:/EDA/COSMIC/CXSTM8
DINCDIR := $(patsubst %,-i%, $(CXDIR)/Hstm8 $(CXDIR)/stm8l15x/inc)
STDLIBDIR := $(CXDIR)/stm8l15x/src
DLIBDIR := $(CXDIR)/Lib 
DDEFS	:= -d STM8L05X_MD_VL
LKF		:= _lkf/stm8.lkf
PROJECT	:= main
SRC 	:= $(wildcard *.c _libs/*.c app/*.c drv/*.c bsp/*.c thirdparty/*/*.c) # */

UINCS = $(patsubst %,./%, _libs app bsp drv)
UINCS += $(patsubst %,./thirdparty/%, xprintf ztask zromfs zuart strtok zevent) 
UINCS += $(patsubst %,./thirdparty/%, zkey)
DINCDIR += $(patsubst %,-i %, $(UINCS))


SRC += $(STDLIBDIR)/stm8l15x_gpio.c
SRC += $(STDLIBDIR)/stm8l15x_spi.c
SRC += $(STDLIBDIR)/stm8l15x_clk.c
SRC += $(STDLIBDIR)/stm8l15x_usart.c
SRC += $(STDLIBDIR)/stm8l15x_syscfg.c
SRC += $(STDLIBDIR)/stm8l15x_tim4.c
#SRC += $(STDLIBDIR)/stm8l15x_adc.c
#SRC += $(STDLIBDIR)/stm8l15x_dma.c
#SRC += $(STDLIBDIR)/stm8l15x_dac.c 
SRC += $(STDLIBDIR)/stm8l15x_rtc.c
SRC += $(STDLIBDIR)/stm8l15x_lcd.c
#SRC += $(STDLIBDIR)/stm8l15x_exti.c
#SRC += $(STDLIBDIR)/stm8l15x_flash.c
SRC += $(STDLIBDIR)/stm8l15x_pwr.c


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
	cxstm8 +mods0 -libf -pxp -no -pp -l $(DDEFS) $(DINCDIR) -cl. -co. $< > /dev/nul

%.elf: $(OBJS) $(LKF)
	clnk -m $(PROJECT).map -l $(DLIBDIR) -o $(PROJECT).sm8 $(LKF)
	cvdwarf $(PROJECT).sm8

%.hex: %.elf
	chex -fi -o $(PROJECT).hex $(PROJECT).sm8

%.lkf: %.lkf.template 
	sed -e "s/%%OBJS%%/$(subst stm8_interrupt_vector.o,,$(notdir $(OBJS)))/" $< > $@

flash:
	$(STVP_CMDLINE) -fileprog=main.hex -verif -no_loop | grep "<<<"

af: all flash

clean:
	$(RM) *.o *.elf *.ls *.hex *.sm8 *.map *~ *.*~ $(LKF)

.PHONY: clean all flash
#.SECONDARY: %.lkf
