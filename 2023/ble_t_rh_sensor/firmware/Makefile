# hardware
CPU := cortex-m23
MCU := GD32E230
FREQ := 16000000
FLASH_SIZE := 56K
MEM_SIZE := 8K

# toolchain
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CPP	 = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
HEX  = $(CP) -O ihex
BIN	 = $(CP) -O binary
REMOVE = rm -f
REMOVEDIR = rm -rf
STLINK := "C:/Program Files (x86)/STMicroelectronics/STM32 ST-LINK Utility/ST-LINK Utility/ST-LINK_CLI.exe" 
DOXYGEN := doxygen
PYTHON := "py.exe"
IAP  := $(PYTHON) "iap.py"

# List all default C defines here, like -D_DEBUG=1

DEBUG = 0
DDEFS = -D$(shell echo $(MCU) | tr a-z A-Z)
DDEFS += -DUSE_STDPERIPH_DRIVER -DHXTAL_VALUE=$(FREQ)		# use HXTAL_VALUE instead of HSE_VALUE
DDEFS += -D_DEBUG=$(DEBUG)
DADEFS =
DINCDIR =
DLIBDIR =
DLIBS =
# End of default section
 
# Start of user section
PROJECT = main
UDEFS =
UADEFS =

# List source files here
STDLIBDIR = d:/work/libs/cmsis/cmsis_gd32e23x_v112
CORELIBDIR = $(STDLIBDIR)/_cmsis
STDPHDRV_DIR = $(STDLIBDIR)/_e23x_stdperiph_drv
STDPHDRV_SRCDIR = $(STDPHDRV_DIR)/src
STDPHDRV_INCDIR = $(STDPHDRV_DIR)/inc
STARTUPDIR = $(STDLIBDIR)/_startup
INCDIR = ./inc
DLIBS = -L. -lm -lstdc++ -fno-use-cxa-atexit#-lgd32f30x_16m

## List ASM source files here
ASRC = $(STARTUPDIR)/startup_$(shell echo $(MCU) | tr A-Z a-z).s

## List C source files here
SRC = $(wildcard $(patsubst %,%/*.c, . _libs app bsp drv dhrystone thirdparty/*))
SRCC = $(wildcard $(patsubst %,%/*.cc, . _libs app bsp drv dhrystone thirdparty/*))

ASRC += $(wildcard *.s _libs/*.s app/*.s bsp/*/*.s thirdparty/*/*.s) #*/

SRC += $(STDPHDRV_SRCDIR)/gd32e23x_rcu.c
SRC += $(STDPHDRV_SRCDIR)/gd32e23x_gpio.c
SRC += $(STDPHDRV_SRCDIR)/gd32e23x_usart.c
SRC += $(STDPHDRV_SRCDIR)/gd32e23x_fmc.c
SRC += $(STDPHDRV_SRCDIR)/gd32e23x_pmu.c
SRC += $(STDPHDRV_SRCDIR)/gd32e23x_misc.c
SRC += $(STDPHDRV_SRCDIR)/gd32e23x_spi.c
SRC += $(STDPHDRV_SRCDIR)/gd32e23x_rtc.c

## List all user directories here
UINCDIR = $(CORELIBDIR) $(DEV_DIR) $(STDPHDRV_INCDIR)
UINCDIR += $(patsubst %,./%, . _libs bsp app drv dhrystone)
UINCDIR += $(patsubst %,./thirdparty/%, xprintf ztask zuart flash_eeprom zi2c ugui)

# List the user directory to look for the libraries here
ULIBDIR = 

# List all user libraries here
ULIBS = 
# Define optimisation level here
OPT = -O2 
# End of user defines

# Define linker script file here
LDSCRIPT = ./_ldscripts/gd32ffprgt_rom.lds
FULL_PRJ = $(PROJECT)_rom
 
INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
LIBDIR  = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))
 
DEFS    = $(DDEFS) $(UDEFS)
ADEFS   = $(DADEFS) $(UADEFS)
OBJS    = $(ASRC:.s=.o) $(SRC:.c=.o) $(SRCC:.cc=.o)
LIBS    = $(DLIBS) $(ULIBS)
MCFLAGS = -mcpu=$(CPU)
 
ASFLAGS = $(MCFLAGS) -g -gdwarf-2 -mthumb  -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CPFLAGS = $(MCFLAGS) $(OPT) -g -gdwarf-2 -mthumb
#CPFLAGS += -Wall -Wstrict-prototypes -Wa,-ahlms=$(<:.c=.lst) $(DEFS)
CPFLAGS += -Wall -Wa,-ahlms=$(<:.c=.lst) $(DEFS)
CPFLAGS += -ffunction-sections -fdata-sections #-std=gnu99
CPPFLAGS = -std=gnu++17 -Wa,-ahlms=$(<:.cc=.lst) $(DEFS) -fno-exceptions
#CPFLAGS += -Wall -Wstrict-prototypes -Wa,-ahlms=$(<:.c=.lst) $(DEFS)
LDFLAGS = $(MCFLAGS) -mthumb -nostartfiles -T$(LDSCRIPT)
LDFLAGS += -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch $(LIBDIR)
LDFLAGS += -fno-common
LDFLAGS += -Wl,--gc-sections -specs=nano.specs #-u _printf_float 
 
TEXT =
# Generate dependency information
#CPFLAGS += -MMD -MP -MF #.dep/$(@F).d
 
# makefile rules
 
.PHONY: all clean
all: $(OBJS) $(LDSCRIPT) $(FULL_PRJ).elf  $(FULL_PRJ).hex $(FULL_PRJ).bin
	$(TRGT)size $(PROJECT)_rom.elf

	@arr=(`$(TRGT)size $(PROJECT)_rom.elf | sed -n '2p'`); \
	let flash=($${arr[0]}+$${arr[1]}); \
	let mem=($${arr[1]}+$${arr[2]}); \
	let flash_size=$(subst K,,$(FLASH_SIZE))*1024; \
	let mem_size=$(subst K,,$(MEM_SIZE))*1024; \
	flash_usage=`echo "scale=1;($$flash*100/$$flash_size)" | bc`; \
	mem_usage=`echo "scale=1;($$mem*100/$$mem_size)" | bc`; \
	echo "Flash: $$flash / $$flash_size bytes, $$flash_usage% Full (.text + .data)"; \
	echo "SRAM:  $$mem / $$mem_size bytes, $$mem_usage% Full (.data + .bss)";
 
%.o: %.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) $< -o $@

%.o: %.cc
	$(CPP) -c $(CPFLAGS) $(CPPFLAGS) -I . $(INCDIR) $< -o $@ 

%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@

%.lds: %.lds.template
	sed -e "s/%%FLASH_SIZE%%/$(FLASH_SIZE)/" -e "s/%%MEM_SIZE%%/$(MEM_SIZE)/" $< > $@

%.elf: $(OBJS) $(LDSCRIPT)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

%.hex: %.elf
	$(HEX) $< $@
	
%.bin: %.elf
	$(BIN) $< $@

clean:
	$(REMOVE) $(OBJS)
	rm $(FULL_PRJ).elf
	rm $(FULL_PRJ).map
	rm $(FULL_PRJ).hex
	rm $(LDSCRIPT)
	rm $(SRC:.c=.lst)
	rm $(ASRC:.s=.lst)

zboot:
	$(STLINK) -c SWD freq=2000 -P zboot.hex -V -Rst

flash:
	$(STLINK) -c SWD freq=2000 -P $(FULL_PRJ).hex -V -Rst

iap:
	$(IAP) $(FULL_PRJ).hex

reset:
	$(STLINK) -c SWD -Rst 
