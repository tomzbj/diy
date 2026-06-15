# hardware
CPU := cortex-m0plus
MCU := PY32F002Ax5
FREQ := 24000000
FLASH_SIZE := 32K
MEM_SIZE := 4K

# toolchain
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CPP  = $(TRGT)g++
CP   = $(TRGT)objcopy
AR	 = $(TRGT)ar -cr 
AS   = $(TRGT)gcc -x assembler-with-cpp
REMOVE = rm -f
PYOCD = "pyocd"

# List all default C defines here, like -D_DEBUG=1
DDEFS = -D$(MCU)
DDEFS += -DUSE_HAL_DRIVER
DDEFS += -DHSE_VALUE=$(FREQ)
DLIBS = -lstdc++ -fno-use-cxa-atexit 

# Start of user section
PROJECT = app

# List source files here
HALDIR = d:/work/libs/cmsis/py32f0xx_v112
CORELIBDIR = $(HALDIR)/_cmsis
HALDRV_DIR = $(HALDIR)/_f0xx_drv
HALDRV_SRCDIR = $(HALDRV_DIR)/src
HALDRV_INCDIR = $(HALDRV_DIR)/inc
STARTUPDIR = $(HALDIR)/_startup

## List ASM source files here
ASRC = $(STARTUPDIR)/startup_$(shell echo $(MCU) | tr A-Z a-z).s

## List C source files here 
SRCC = $(wildcard $(patsubst %,%/*.cc, . _libs app bsp drv thirdparty/*))
SRC = $(wildcard $(patsubst %,%/*.c, . _libs app bsp drv thirdparty/*))

SRC_HALDRV = $(wildcard $(HALDRV_SRCDIR)/*.c)
OBJS_HALDRV = $(SRC_HALDRV:.c=.o) 

## List all user directories here
UINCDIR = ./_libs $(CORELIBDIR) $(DEV_DIR) $(HALDRV_INCDIR)
UINCDIR += $(patsubst %,./%, bsp drv app)
UINCDIR += $(patsubst %,./thirdparty/%, zuart zpin zfifo zcli ztask zcharlie)

OPT = -Os 
# End of user defines

# Define linker script file here
LDSCRIPT = ./_ldscripts/py32f0xx_rom.lds
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
CFLAGS = $(CFLAGS_COMMON) -Wstrict-prototypes -Wstrict-prototypes -Wa,-ahlms=$(<:.c=.lst) $(DEFS) -std=gnu99
CFLAGS += -Wno-maybe-uninitialized
CPPFLAGS = $(CFLAGS_COMMON) -Wa,-ahlms=$(<:.cc=.lst) $(DEFS) -std=gnu++17 -fno-exceptions -Wno-register 
LDFLAGS = $(MCFLAGS) -mthumb -T$(LDSCRIPT)
LDFLAGS += -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch $(LIBDIR)
LDFLAGS += -Wl,--gc-sections -specs=nano.specs #-u _printf_float
LDFLAGS += -L. -lpy32f002
 
# makefile rules
all: $(OBJS) $(LDSCRIPT) $(FULL_PRJ).elf
	$(TRGT)size -B $(PROJECT).elf 
	@arr=(`$(TRGT)size $(PROJECT).elf | sed -n '2p'`); \
	let flash=($${arr[0]}+$${arr[1]}); \
	let mem=($${arr[1]}+$${arr[2]}); \
	let flash_size=$(subst K,,$(FLASH_SIZE))*1024; \
	let mem_size=$(subst K,,$(MEM_SIZE))*1024; \
	flash_usage=`echo "scale=1;($$flash*100/$$flash_size)" | bc`; \
	mem_usage=`echo "scale=1;($$mem*100/$$mem_size)" | bc`; \
	printf "FLASH:%5d/%5d,%5.1f%% (text+data)\n" $$flash $$flash_size $$flash_usage; \
	printf "SRAM: %5d/%5d,%5.1f%% (data+bss) \n" $$mem $$mem_size $$mem_usage

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
libpy32f002.a: $(OBJS_HALDRV)
	$(AR) $@ $(OBJS_HALDRV)
	@echo $@ done. 
flash:
	$(PYOCD) flash -t py32f002ax5 $(FULL_PRJ).elf
reset:
	$(PYOCD) reset -t py32f002ax5
erase:
	$(PYOCD) erase -c -t py32f002ax5
clean:
	$(REMOVE) $(OBJS)
	rm $(FULL_PRJ).elf $(FULL_PRJ).map $(FULL_PRJ).hex $(LDSCRIPT)
	rm $(SRC:.c=.lst) $(ASRC:.s=.lst)