# hardware
CPU := cortex-m0
MCU := STM32F072
FREQ := 8000000
FLASH_SIZE := 118K
MEM_SIZE := 16K

# toolchain
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
HEX  = $(CP) -O ihex
REMOVE = rm -f
REMOVEDIR = rm -rf
STLINK := "C:/Program Files (x86)/STMicroelectronics/STM32 ST-LINK Utility/ST-LINK Utility/ST-LINK_CLI.exe"
PYTHON := "py.exe"
IAP  := $(PYTHON) "iap.py"

# List all default C defines here, like -D_DEBUG=1
DDEFS = -D$(shell echo $(MCU) | tr a-z A-Z)
#DDEFS = -D$(MCU)
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
STDLIBDIR = c:/yagarto/cmsis_cm0_v150
CORELIBDIR = $(STDLIBDIR)/_cmsis
STDPHDRV_DIR = $(STDLIBDIR)/_f0xx_stdperiph_drv
STDPHDRV_SRCDIR = $(STDPHDRV_DIR)/src
STDPHDRV_INCDIR = $(STDPHDRV_DIR)/inc
STARTUPDIR = $(STDLIBDIR)/_startup

USB_DIR = $(STDLIBDIR)/_usb

## List ASM source files here
ASRC = $(STARTUPDIR)/startup_$(shell echo $(MCU) | tr A-Z a-z).s

## List C source files here
SRC = $(wildcard *.c _libs/*.c app/*.c drv/*.c bsp/*.c usb/*.c thirdparty/*/*.c) 

## List all user directories here
UINCDIR = ./_libs $(CORELIBDIR) $(DEV_DIR) $(STDPHDRV_INCDIR)
UINCDIR += $(patsubst %,./%, bsp drv app usb)
UINCDIR += $(patsubst %,./thirdparty/%, ztask zkey ugui zi2c flash_eeprom)

########################################### for usb
SRC += $(wildcard $(USB_DIR)/usbd_core/src/*.c)
SRC += $(wildcard $(USB_DIR)/usbd_drv/src/*.c)
#SRC += $(wildcard $(USB_DIR)/usbd_classes/hid/src/*.c) 
SRC += $(wildcard $(USB_DIR)/usbd_classes/msc/src/*.c) 
UINCDIR += $(USB_DIR)/usbd_core/inc
UINCDIR += $(USB_DIR)/usbd_drv/inc
#UINCDIR += $(USB_DIR)/usbd_classes/hid/inc
UINCDIR += $(USB_DIR)/usbd_classes/msc/inc

# List the user directory to look for the libraries here
ULIBDIR = 
# List all user libraries here
ULIBS = 
# Define optimisation level here
OPT = -Os 
# End of user defines

# Define linker script file here
LDSCRIPT = ./_ldscripts/stm32f0xx_rom.lds
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
CPFLAGS += -Wno-unused-function
CPFLAGS += -ffunction-sections -fdata-sections -std=gnu99
LDFLAGS = $(MCFLAGS) -mthumb -nostartfiles -T$(LDSCRIPT)
LDFLAGS += -L. -lstm32f072_16m
LDFLAGS += -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch $(LIBDIR)
LDFLAGS += -Wl,--gc-sections -specs=nano.specs #-u _printf_float
 
# Generate dependency information
#CPFLAGS += -MMD -MP -MF .dep/$(@F).d
 
# makefile rules
 
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
	echo "SRAM:  $$mem / $$mem_size bytes, $$mem_usage% Full (.data + .bss)"; 

%.o: %.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) $< -o $@

%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@

%.lds: %.lds.template
	sed -e "s/%%FLASH_SIZE%%/$(FLASH_SIZE)/" -e "s/%%MEM_SIZE%%/$(MEM_SIZE)/" $< > $@

%.elf: $(OBJS) $(LDSCRIPT)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

%.hex: %.elf
	$(HEX) $< $@

flash:
	$(STLINK) -c SWD -P $(FULL_PRJ).hex -V -Rst 

iap:
	$(IAP) $(FULL_PRJ).hex

reset:
	$(STLINK) -c SWD -Rst 
	
clean:
	$(REMOVE) $(OBJS)
	rm $(FULL_PRJ).elf
	rm $(FULL_PRJ).map
	rm $(FULL_PRJ).hex
	rm $(LDSCRIPT)
	rm $(SRC:.c=.lst)
	rm $(ASRC:.s=.lst)
