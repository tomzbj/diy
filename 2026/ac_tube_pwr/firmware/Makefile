# hardware
CPU := cortex-m0plus
MCU := PY32MD320x8
FREQ := 48000000
FLASH_SIZE := 64K
MEM_SIZE := 8K

# toolchain
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CPP  = $(TRGT)g++
AR	 = $(TRGT)ar -cr 
AS   = $(TRGT)gcc -x assembler-with-cpp
REMOVE = rm -f
PYOCD = "pyocd"

# List all default C defines here, like -D_DEBUG=1
DDEFS = -D$(MCU)
DDEFS += -DUSE_HAL_DRIVER -DDMA
DDEFS += -DHSE_VALUE=$(FREQ)
DLIBS = -lstdc++ -fno-use-cxa-atexit 
PROJECT = app

# List source files here
HALDIR = D:/work/libs/cmsis/py32md320_v050
CORELIBDIR = $(HALDIR)/_cmsis
HALDRV_DIR = $(HALDIR)/_md3xx_hal_drv
HALDRV_SRCDIR = $(HALDRV_DIR)/src
HALDRV_INCDIR = $(HALDRV_DIR)/inc
STARTUPDIR = $(HALDIR)/_startup

## List ASM source files here
ASRC = $(STARTUPDIR)/startup_py32md320xx.s

## List C source files here 
SRC = $(wildcard *.c _libs/*.c app/*.c drv/*.c usb/*.c bsp/*.c thirdparty/*/*.c)
SRCC = $(wildcard $(patsubst %,%/*.cc, . _libs app bsp drv thirdparty/*))

SRC_HALDRV = $(wildcard $(HALDRV_SRCDIR)/*.c)
OBJS_HALDRV = $(SRC_HALDRV:.c=.o) 

## List all user directories here
UINCDIR = ./_libs $(CORELIBDIR) $(DEV_DIR) $(HALDRV_INCDIR)
UINCDIR += $(patsubst %,./%, bsp drv app)
UINCDIR += $(patsubst %,./thirdparty/%, zuart zpin zfifo zi2c zcli ztask ugui zevent zkey)
OPT = -Os

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
CFLAGS_COMMON += -Wall -ffunction-sections -fdata-sections -Wno-comment
CFLAGS = $(CFLAGS_COMMON) -Wstrict-prototypes -Wstrict-prototypes -Wa,-ahlms=$(<:.c=.lst) $(DEFS) -std=gnu17
CFLAGS += -Wno-maybe-uninitialized
CPPFLAGS = $(CFLAGS_COMMON) -Wa,-ahlms=$(<:.cc=.lst) $(DEFS) -std=gnu++17 -fno-exceptions -Wno-register 
LDFLAGS = $(MCFLAGS) -mthumb -T$(LDSCRIPT)
LDFLAGS += -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch $(LIBDIR)
LDFLAGS += -Wl,--gc-sections -specs=nano.specs -u _printf_float
LDFLAGS += -L. -lpy32md320
 
# makefile rules
all: $(OBJS) $(LDSCRIPT) $(FULL_PRJ).elf
#	$(TRGT)size -B $(PROJECT).elf 
	@arr=(`$(TRGT)size $(PROJECT).elf | sed -n '2p'`); \
	let flash=($${arr[0]}+$${arr[1]}); \
	let mem=($${arr[1]}+$${arr[2]}); \
	let flash_size=$(subst K,,$(FLASH_SIZE))*1024; \
	let mem_size=$(subst K,,$(MEM_SIZE))*1024; \
	flash_usage=`echo "scale=1;($$flash*100/$$flash_size)" | bc`; \
	mem_usage=`echo "scale=1;($$mem*100/$$mem_size)" | bc`; \
	printf "FLASH:%6d/%6d,%5.1f%% \n" $$flash $$flash_size $$flash_usage; \
	printf "SRAM: %6d/%6d,%5.1f%% \n" $$mem $$mem_size $$mem_usage

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
libpy32md320.a: $(OBJS_HALDRV)
	$(AR) $@ $(OBJS_HALDRV)
	@echo $@ done. 
flash:
	$(PYOCD) flash -t py32md320x8 $(FULL_PRJ).elf
reset:
	$(PYOCD) reset -t py32md320x8
erase:
	$(PYOCD) erase -c -t py32md320x8
clean:
	$(REMOVE) $(OBJS)
	rm $(FULL_PRJ).elf $(FULL_PRJ).map $(FULL_PRJ).hex $(LDSCRIPT)
	rm $(SRC:.c=.lst) $(ASRC:.s=.lst)
