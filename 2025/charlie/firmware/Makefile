# hardware
CPU := cortex-m4
MCU := GD32F405
FREQ := 16000000
FLASH_SIZE := 512K
MEM_SIZE := 128K
 # 64k TCM

# toolchain
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CPP   = $(TRGT)g++
CP   = $(TRGT)objcopy
BIN	 = $(TRGT)objcopy -O binary
AR	 = $(TRGT)ar -cr 
AS   = $(TRGT)gcc -x assembler-with-cpp
HEX  = $(CP) -O ihex
REMOVE = rm -f
REMOVEDIR = rm -rf
PYTHON := "py.exe"
IAP  := $(PYTHON) "iap.py"
PYOCD := pyocd.exe

# List all default C defines here, like -D_DEBUG=1 
DEBUG = 0
DDEFS = -D$(shell echo $(MCU) | tr a-z A-Z)
DDEFS += -DUSE_STDPERIPH_DRIVER -DHXTAL_VALUE=$(FREQ)		# use HXTAL_VALUE instead of HSE_VALUE
DDEFS += -D_DEBUG=$(DEBUG)

USBDIR = _f4xx_usb_drv
# End of default section
 
# Start of user section
PROJECT = app
UDEFS =
UADEFS =

# List source files here
STDLIBDIR = d:/work/libs/cmsis/cmsis_gd32f4xx_v320
CORELIBDIR = $(STDLIBDIR)/_cmsis
STDPHDRV_DIR = $(STDLIBDIR)/_f4xx_stdperiph_drv
STDPHDRV_SRCDIR = $(STDPHDRV_DIR)/src
STDPHDRV_INCDIR = $(STDPHDRV_DIR)/inc
STARTUPDIR = $(STDLIBDIR)/_startup
INCDIR = ./inc
DLIBS = -L. -lstdc++  #-lm -lgd32f30x_16m
DLIBS += -lgd32f40x

## List ASM source files here
ASRC = $(STARTUPDIR)/startup_$(shell echo $(MCU) | tr A-Z a-z).s

## List C source files here
SRC = $(wildcard $(patsubst %,%/*.c, . _libs app bsp drv usb thirdparty/*))
SRCC = $(wildcard $(patsubst %,%/*.cc, . _libs app bsp drv usb thirdparty/*))
ASRC += $(wildcard *.s _libs/*.s app/*.s bsp/*/*.s thirdparty/*/*.s) #*/
SRC += $(wildcard  cocoos/src/*.c) 

SRC_STDPHDRV += $(wildcard $(STDPHDRV_SRCDIR)/*.c)
OBJS_STDPHDRV = $(SRC_STDPHDRV:.c=.o)

## List all user directories here
UINCDIR = $(CORELIBDIR) $(DEV_DIR) $(STDPHDRV_INCDIR)
UINCDIR += $(patsubst %,./%, . _libs bsp app drv usb)
UINCDIR += $(patsubst %,./thirdparty/%, zfifo zcli zuart flash_eeprom zpin ugui)
UINCDIR += $(patsubst %,./thirdparty/%, flasher crc32)
UINCDIR += ./cocoos/inc 

# List the user directory to look for the libraries here
ULIBDIR = 

# List all user libraries here
ULIBS = 
# Define optimisation level here
OPT = -O3 
# End of user defines

# Define linker script file here
LDSCRIPT = ./_ldscripts/gd32ffprgt_rom.lds
FULL_PRJ = $(PROJECT)
 
INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
LIBDIR  = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))
 
DEFS    = $(DDEFS) $(UDEFS)
ADEFS   = $(DADEFS) $(UADEFS)
OBJS    = $(ASRC:.s=.o) $(SRC:.c=.o) $(SRCC:.cc=.o)
LIBS    = $(DLIBS) $(ULIBS)
MCFLAGS = -mcpu=$(CPU)
 
ASFLAGS = $(MCFLAGS) -g -gdwarf-2 -mthumb  -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CFLAGS_COMMON = $(MCFLAGS) $(OPT) -g -gdwarf-2 -mthumb
CFLAGS_COMMON += -Wall -ffunction-sections -fdata-sections 
CFLAGS = $(CFLAGS_COMMON) -Wa,-ahlms=$(<:.c=.lst) $(DEFS) -Wstrict-prototypes -std=gnu17 -Wno-maybe-uninitialized
CPPFLAGS = $(CFLAGS_COMMON) -Wa,-ahlms=$(<:.cc=.lst) $(DEFS) -std=gnu++17 -fno-exceptions -Wno-register
LDFLAGS = $(MCFLAGS) -mthumb -nostartfiles -T$(LDSCRIPT)
LDFLAGS += -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch $(LIBDIR)
LDFLAGS += -fno-common
LDFLAGS += -Wl,--gc-sections -specs=nano.specs #-u _printf_float 
 
# makefile rules 
.PHONY: all clean
all: $(OBJS) $(LDSCRIPT) $(FULL_PRJ).elf
	$(TRGT)size $(PROJECT).elf 
	@arr=(`$(TRGT)size $(PROJECT).elf | sed -n '2p'`); \
	let flash=($${arr[0]}+$${arr[1]}); \
	let mem=($${arr[1]}+$${arr[2]}); \
	let flash_size=$(subst K,,$(FLASH_SIZE))*1024; \
	let mem_size=$(subst K,,$(MEM_SIZE))*1024; \
	flash_usage=`echo "scale=1;($$flash*100/$$flash_size)" | bc`; \
	mem_usage=`echo "scale=1;($$mem*100/$$mem_size)" | bc`; \
  printf "FLASH: %6d/%6d, %5.1f%% (.text + .data)\n" $$flash $$flash_size $$flash_usage; \
	printf "SRAM:  %6d/%6d, %5.1f%% (.data + .bss) \n" $$mem $$mem_size $$mem_usage

libgd32f40x.a: $(OBJS_STDPHDRV)
	$(AR) $@ $(OBJS_STDPHDRV)
	@echo $@ done.  
%.o: %.c
	$(CC) -c $(CFLAGS) -I . $(INCDIR) $< -o $@ 
%.o: %.cc
	$(CPP) -c $(CPPFLAGS) -I . $(INCDIR) $< -o $@ 
%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@ 
%.lds: %.lds.template
	sed -e "s/%%FLASH_SIZE%%/$(FLASH_SIZE)/" -e "s/%%MEM_SIZE%%/$(MEM_SIZE)/" $< > $@ 
%.elf: $(OBJS) $(LDSCRIPT)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@ 
clean:
	$(REMOVE) $(OBJS) $(FULL_PRJ).elf $(FULL_PRJ).map $(LDSCRIPT)
	$(REMOVE) $(SRC:.c=.lst) $(SRC:.cc=.lst) $(ASRC:.s=.lst) 

flash:
	$(PYOCD) flash $(FULL_PRJ).elf -t gd32f405rg
reset:
	$(PYOCD) reset -t gd32f405rg
