# hardware
CPU := cortex-m4
MCU := STM32F103xE
FREQ := 16000000
FLASH_SIZE := 256K
MEM_SIZE := 48K

# toolchain
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CPP	 = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
AR	 = $(TRGT)ar -cr 
REMOVE = rm -f
PYTHON := "py.exe"
PYOCD = "pyocd"

# List all default C defines here, like -D_DEBUG=1 
DEBUG = 0
DDEFS = -D $(MCU)#$(shell echo $(MCU) | tr a-z A-Z)
DDEFS += -DUSE_HAL_DRIVER -DHSE_VALUE=$(FREQ)
DDEFS += -D_DEBUG=$(DEBUG)
 
# Start of user section
PROJECT = app
HALDIR = d:/work/libs/cmsis/hal_f1
CORELIBDIR = $(HALDIR)/_cmsis
HALDRV_DIR = $(HALDIR)/_f1xx_hal_drv
HALDRV_SRCDIR = $(HALDRV_DIR)/src
HALDRV_INCDIR = $(HALDRV_DIR)/inc
STARTUPDIR = $(HALDIR)/_startup

## List ASM source files here
ASRC = $(STARTUPDIR)/startup_$(shell echo $(MCU) | tr A-Z a-z).s
ASRC += $(wildcard *.s _libs/*.s app/*.s bsp/*/*.s thirdparty/*/*.s) #*/
SRC = $(wildcard $(patsubst %,%/*.c, . _libs app bsp drv thirdparty/*))
SRCC = $(wildcard $(patsubst %,%/*.cc, . _libs app bsp drv thirdparty/*))

SRC_HALDRV = $(wildcard $(HALDRV_SRCDIR)/*.c)
OBJS_HALDRV = $(SRC_HALDRV:.c=.o) 

## List all user directories here
UINCDIR = $(CORELIBDIR) $(DEV_DIR) $(HALDRV_INCDIR)
UINCDIR += $(patsubst %,./%, . _libs bsp app drv)
UINCDIR += $(patsubst %,./thirdparty/%, ztask zuart zpin zcli zfifo)

# Define optimisation level here
OPT = -O3 

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

CFLAGS = $(CFLAGS_COMMON) -Wall -Wa,-ahlms=$(<:.c=.lst) $(DEFS) 
CFLAGS += -ffunction-sections -fdata-sections -std=gnu17 
CPPFLAGS = $(CFLAGS_COMMON) -std=gnu++17 -Wa,-ahlms=$(<:.cc=.lst) $(DEFS) -fno-exceptions

LDFLAGS = $(MCFLAGS) -mthumb -nostartfiles -T$(LDSCRIPT)
LDFLAGS += -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch $(LIBDIR)
LDFLAGS += -fno-common
LDFLAGS += -Wl,--gc-sections -specs=nano.specs -u _printf_float 
LDFLAGS += -L. -lstm32f103
 
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
libstm32f103.a: $(OBJS_HALDRV)
	$(AR) $@ $(OBJS_HALDRV)
	@echo $@ done. 
clean:
	$(REMOVE) $(OBJS)
	rm $(FULL_PRJ).elf $(FULL_PRJ).map $(LDSCRIPT) $(SRC:.c=.lst) $(ASRC:.s=.lst) 
flash:
	$(PYOCD) flash -t gd32f303cc $(FULL_PRJ).elf -f 500k
reset:
	$(PYOCD) reset -t gd32f303cc 
