# hardware
CPU := cortex-m4
MCU := GD32F30X_HD
FREQ := 16000000
FLASH_SIZE := 246K
MEM_SIZE := 96K

# toolchain
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CPP	 = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
HEX  = $(CP) -O ihex
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
 
# Start of user section
PROJECT = main

# List source files here
STDLIBDIR = d:/work/libs/cmsis/cmsis_gd32f30x_v211
CORELIBDIR = $(STDLIBDIR)/_cmsis
STDPHDRV_DIR = $(STDLIBDIR)/_f30x_stdperiph_drv
STDPHDRV_SRCDIR = $(STDPHDRV_DIR)/src
STDPHDRV_INCDIR = $(STDPHDRV_DIR)/inc
STARTUPDIR = $(STDLIBDIR)/_startup
INCDIR = ./inc
DLIBS = -L. -lm -lgd32f30x #-larm_cortexM4l_math

## List ASM source files here
ASRC = $(STARTUPDIR)/startup_$(shell echo $(MCU) | tr A-Z a-z).s

## List C source files here
SRC = $(wildcard $(patsubst %,%/*.c, . _libs app bsp drv thirdparty/*))
SRCC = $(wildcard $(patsubst %,%/*.cc, . _libs app bsp drv thirdparty/*))
ASRC += $(wildcard *.s _libs/*.s app/*.s bsp/*/*.s thirdparty/*/*.s) #*/

SRC += $(STDPHDRV_SRCDIR)/gd32f30x_rcu.c


## List all user directories here
UINCDIR = $(CORELIBDIR) $(DEV_DIR) $(STDPHDRV_INCDIR)
UINCDIR += $(patsubst %,./%, . _libs bsp app drv )
UINCDIR += $(patsubst %,./thirdparty/%, ztask zuart flash_eeprom zi2c zpin zcli ugui zhz)
OPT = -O2 # Define optimisation level here
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
MCPU = -mcpu=$(CPU)
 
ASFLAGS = $(MCPU) -g -gdwarf-2 -mthumb  -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CFLAGS_COMMON = $(MCPU) $(OPT) -g -gdwarf-2 -mthumb -Wall $(DEFS)
CFLAGS_COMMON += -ffunction-sections -fdata-sections
CFLAGS = $(CFLAGS_COMMON) -std=gnu11 -Wstrict-prototypes -Wa,-ahlms=$(<:.c=.lst)
CPPFLAGS = $(CFLAGS_COMMON) -std=gnu++11 -Wa,-ahlms=$(<:.cc=.lst) $(DEFS) -fno-exceptions
#CPPFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

LDFLAGS = $(MCPU) -mthumb -nostartfiles -T$(LDSCRIPT)
LDFLAGS += -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch $(LIBDIR)
LDFLAGS += -fno-common -Wl,--gc-sections -specs=nano.specs -u _printf_float -lstdc++ -fno-use-cxa-atexit 

 
# makefile rules
.PHONY: all clean
all: $(OBJS) $(LDSCRIPT) $(FULL_PRJ).elf  $(FULL_PRJ).hex
	$(TRGT)size $(PROJECT)_rom.elf
	
	@arr=(`$(TRGT)size $(PROJECT)_rom.elf | sed -n '2p'`); \
	let flash=($${arr[0]}+$${arr[1]}); \
	let mem=($${arr[1]}+$${arr[2]}); \
	let flash_size=$(subst K,,$(FLASH_SIZE))*1024; \
	let mem_size=$(subst K,,$(MEM_SIZE))*1024; \
	flash_usage=`echo "scale=1;($$flash*100/$$flash_size)" | bc`; \
	mem_usage=`echo "scale=1;($$mem*100/$$mem_size)" | bc`; \
	echo "Flash: $$flash / $$flash_size bytes, $$flash_usage% Full (.text + .data)"; \
	echo "SRAM:  $$mem / $$mem_size bytes, $$mem_usage% Full (.data + .bss)"	

%.o: %.c
	$(CC) -c $(CFLAGS) -I . $(INCDIR) $< -o $@
%.o: %.cc
	$(CPP) -c $(CPPFLAGS) -I . $(INCDIR) $< -o $@ 

%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@
%.lds: %.lds.template
	sed -e "s/%%FLASH_SIZE%%/$(FLASH_SIZE)/" -e "s/%%MEM_SIZE%%/$(MEM_SIZE)/" $< > $@
%.elf: $(OBJS) $(LDSCRIPT)
	$(CPP) $(OBJS) $(LDFLAGS) $(LIBS) -o $@
%.hex: %.elf
	$(HEX) $< $@
%.bin: %.elf
	$(BIN) $< $@
clean:
	$(REMOVE) $(OBJS)
	rm $(FULL_PRJ).elf $(FULL_PRJ).map $(FULL_PRJ).hex $(LDSCRIPT) $(SRC:.c=.lst) $(ASRC:.s=.lst)
zboot:
	$(STLINK) -c SWD freq=4000 -P zboot.hex -V -Rst
flash:
	$(STLINK) -c SWD freq=4000 -P $(FULL_PRJ).hex -V -Rst
iap:
	$(IAP) $(FULL_PRJ).hex
reset:
	$(STLINK) -c SWD -Rst 
