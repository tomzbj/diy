# hardware
CPU := cortex-m0
MCU := STM32F072xB
FREQ := 16000000
FLASH_SIZE := 128K
MEM_SIZE := 16K

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
GDLINK := "d:/dropbox/program/gdlink/gdlink.exe"
PYTHON := "py.exe"
IAP  := $(PYTHON) "iap.py"
PYOCD := "pyocd.exe"

# List all default C defines here, like -D_DEBUG=1 
DEBUG = 0
DDEFS = -D $(MCU)#$(shell echo $(MCU) | tr a-z A-Z)
#DDEFS += -D USE_FULL_LL_DRIVER -D HSE_VALUE=$(FREQ)	
DDEFS += -D HSE_VALUE=$(FREQ)	
DDEFS += -D_DEBUG=$(DEBUG)
 
# Start of user section
PROJECT = app
UDEFS =
UADEFS =

# List source files here
STDLIBDIR = d:/work/libs/cmsis/hal_f0
CORELIBDIR = $(STDLIBDIR)/_cmsis
STDPHDRV_DIR = $(STDLIBDIR)/_f0xx_hal_drv
STDPHDRV_SRCDIR = $(STDPHDRV_DIR)/src
STDPHDRV_INCDIR = $(STDPHDRV_DIR)/inc
STARTUPDIR = $(STDLIBDIR)/_startup
INCDIR = ./inc
DLIBS = -L. -lm -lstdc++ -fno-use-cxa-atexit#-lgd32f30x_16m

## List ASM source files here
ASRC = $(STARTUPDIR)/startup_stm32f072xb.s

## List C source files here
SRC = $(wildcard $(patsubst %,%/*.c, . _libs app bsp drv thirdparty/*))
SRCC = $(wildcard $(patsubst %,%/*.cc, . _libs app bsp drv thirdparty/*))

ASRC += $(wildcard *.s _libs/*.s app/*.s bsp/*/*.s thirdparty/*/*.s) #*/

SRC += $(STDPHDRV_SRCDIR)/stm32f0xx_hal.c
SRC += $(STDPHDRV_SRCDIR)/stm32f0xx_hal_gpio.c
SRC += $(STDPHDRV_SRCDIR)/stm32f0xx_hal_rcc.c
SRC += $(STDPHDRV_SRCDIR)/stm32f0xx_hal_rcc_ex.c
SRC += $(STDPHDRV_SRCDIR)/stm32f0xx_hal_adc.c
SRC += $(STDPHDRV_SRCDIR)/stm32f0xx_hal_adc_ex.c
SRC += $(STDPHDRV_SRCDIR)/stm32f0xx_hal_cortex.c
SRC += $(STDPHDRV_SRCDIR)/stm32f0xx_hal_uart.c
SRC += $(STDPHDRV_SRCDIR)/stm32f0xx_hal_dma.c
SRC += $(STDPHDRV_SRCDIR)/stm32f0xx_hal_pwr.c
SRC += $(STDPHDRV_SRCDIR)/stm32f0xx_hal_rtc.c
SRC += $(STDPHDRV_SRCDIR)/stm32f0xx_hal_rtc_ex.c

SRC += $(wildcard  cocoos/src/*.c) 

## List all user directories here
UINCDIR = $(CORELIBDIR) $(DEV_DIR) $(STDPHDRV_INCDIR)
UINCDIR += $(patsubst %,./%, . _libs bsp app drv)
UINCDIR += $(patsubst %,./thirdparty/%, ztask zuart flash_eeprom zpin zcli zi2c zfifo zcrc32)
UINCDIR += ./cocoos/inc

# List the user directory to look for the libraries here
ULIBDIR = 

# List all user libraries here
ULIBS = 
# Define optimisation level here
OPT = -Os 
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
CFLAGS_COMMON = $(MCFLAGS) $(OPT) -g -gdwarf-2 -mthumb -Wall $(DEFS)
CFLAGS = $(CFLAGS_COMMON) -Wa,-ahlms=$(<:.c=.lst) -Wstrict-prototypes -ffunction-sections -fdata-sections -std=gnu99
CPPFLAGS = $(CFLAGS_COMMON) -std=gnu++17 -Wa,-ahlms=$(<:.cc=.lst) -fno-exceptions -Wno-register
LDFLAGS = $(MCFLAGS) -mthumb -nostartfiles -T$(LDSCRIPT)
LDFLAGS += -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch $(LIBDIR)
LDFLAGS += -fno-common  -Wl,--gc-sections -specs=nano.specs #-u _printf_float 
 
TEXT =
# makefile rules 
.PHONY: all clean reset
all: $(OBJS) $(LDSCRIPT) $(FULL_PRJ).elf  $(FULL_PRJ).hex
	$(TRGT)size $(PROJECT).elf

	@arr=(`$(TRGT)size $(PROJECT).elf | sed -n '2p'`); \
	let flash=($${arr[0]}+$${arr[1]}); \
	let mem=($${arr[1]}+$${arr[2]}); \
	let flash_size=$(subst K,,$(FLASH_SIZE))*1024; \
	let mem_size=$(subst K,,$(MEM_SIZE))*1024; \
	flash_usage=`echo "scale=1;($$flash*100/$$flash_size)" | bc`; \
	mem_usage=`echo "scale=1;($$mem*100/$$mem_size)" | bc`; \
	echo "Flash: $$flash / $$flash_size bytes, $$flash_usage% Full (.text + .data)"; \
	echo "SRAM:  $$mem / $$mem_size bytes, $$mem_usage% Full (.data + .bss)";
 
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
%.hex: %.elf
	$(HEX) $< $@ 
clean:
	$(REMOVE) $(OBJS) *.elf *.map *.hex $(SRC:.c=.lst) $(ASRC:.s=.lst) 
zboot:
	$(PYOCD) flash zboot.hex -t stm32f072cb 
flash:
	$(PYOCD) flash $(FULL_PRJ).hex -t stm32f072cb 
erase:
	$(PYOCD) erase --chip -t stm32f072cb 
iap:
	$(IAP) $(FULL_PRJ).hex 
reset:
	$(PYOCD) reset -t stm32f072cb