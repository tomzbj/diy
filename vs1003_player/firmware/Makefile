# Author:	Zhang Hao
# Date:		2015.10.8

# hardware
CPU := cortex-m3
MCU := STM32F10X_MD
FREQ := 16000000
FLASH_SIZE := 64K
MEM_SIZE := 20K

# toolchain
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
OBJDUMP = $(TRGT)objdump
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary -S --gap-fill 0xff
REMOVE = rm -f
REMOVEDIR = rm -rf
STLINK := "C:/Program Files (x86)/STMicroelectronics/STM32 ST-LINK Utility/ST-LINK Utility/ST-LINK_CLI.exe"
PYTHON := "py.exe"
IAP	 := $(PYTHON) "__console/iap_test.py"

# List all default C defines here, like -D_DEBUG=1
DDEFS = -D$(shell echo $(MCU) | tr a-z A-Z)
DDEFS += -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=$(FREQ)
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
STDLIBDIR = c:/yagarto/cmsis_cm3_v361
CORELIBDIR = $(STDLIBDIR)/_cmsis
STDPHDRV_DIR = $(STDLIBDIR)/_f10x_stdperiph_drv
STDPHDRV_SRCDIR = $(STDPHDRV_DIR)/src
STDPHDRV_INCDIR = $(STDPHDRV_DIR)/inc
STARTUPDIR = $(STDLIBDIR)/_startup

## List ASM source files here
ASRC = $(STARTUPDIR)/startup_$(shell echo $(MCU) | tr A-Z a-z).s
# ASRC += libs/fonts.s

## List C source files here
SRC = $(wildcard *.c _libs/*.c app/*/*.c hw/*/*.c usb/*.c usb/usblib/*.c)
SRC += $(wildcard thirdparty/fatfs/*.c)

## List all user directories here
UINCDIR = ./_libs $(CORELIBDIR) $(DEV_DIR) $(STDPHDRV_INCDIR)
UINCDIR += ./usb ./usb/usblib
UINCDIR += $(patsubst %,./hw/%, usart vs1003 spiflash pwr)
UINCDIR += $(patsubst %,./app/%, cmd playback)
UINCDIR += $(patsubst %,./thirdparty/%, bget fatfs)

# List the user directory to look for the libraries here
ULIBDIR = 
# List all user libraries here
ULIBS = 
# Define optimisation level here
OPT = -Os 
# End of user defines

# Define linker script file here
LDSCRIPT = ./_ldscripts/stm32f10x_rom.lds
FULL_PRJ = $(PROJECT)_rom
 
INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
LIBDIR  = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))
 
DEFS    = $(DDEFS) $(UDEFS)
ADEFS   = $(DADEFS) $(UADEFS)
OBJS    = $(ASRC:.s=.o) $(SRC:.c=.o)
LIBS    = $(DLIBS) $(ULIBS)
MCFLAGS = -mcpu=$(CPU)
 
ASFLAGS = $(MCFLAGS) -g -gdwarf-2 -mthumb  -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CPFLAGS = $(MCFLAGS) $(OPT) -g -gdwarf-2 -mthumb
CPFLAGS += -Wall -Wstrict-prototypes -Wa,-ahlms=$(<:.c=.lst) $(DEFS)
CPFLAGS += -ffunction-sections -fdata-sections -std=gnu99 
LDFLAGS = $(MCFLAGS) -mthumb -nostartfiles -T$(LDSCRIPT)
LDFLAGS += -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch $(LIBDIR)
LDFLAGS += -Wl,--gc-sections -specs=nano.specs
LDFLAGS += -L. -lstm32f10x_md_16m
# Generate dependency information
#CPFLAGS += -MMD -MP -MF #.dep/$(@F).d
 
# makefile rules
 
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

%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@

%.lds: %.lds.template
	sed -e "s/%%FLASH_SIZE%%/$(FLASH_SIZE)/" -e "s/%%MEM_SIZE%%/$(MEM_SIZE)/" $< > $@

%.elf: $(OBJS) $(LDSCRIPT)
	@$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

%.lss: %.elf
	$(OBJDUMP) -h -S $< > $@

%.hex: %.elf
	$(HEX) $< $@

%.bin: %.elf
	$(BIN) $< $@
	
flash:
	$(STLINK) -c SWD freq=4000 -P $(FULL_PRJ).hex -V -Rst

iap:
	$(IAP) $(FULL_PRJ).bin

reset:
	$(STLINK) -c SWD -Rst 

clean:
	$(REMOVE) $(OBJS)
	rm $(FULL_PRJ).elf
	rm $(FULL_PRJ).map
	rm $(FULL_PRJ).hex
#	rm $(FULL_PRJ).bin
	rm $(LDSCRIPT)
	rm $(SRC:.c=.lst)
	rm $(ASRC:.s=.lst)
    
# Include the dependency files, should be the last of the makefile
#
#-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
