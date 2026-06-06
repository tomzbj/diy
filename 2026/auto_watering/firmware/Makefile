# hardware
CPU := cortex-m4
MCU := STM32F103xE
FREQ := 8000000
FLASH_SIZE := 500K
MEM_SIZE := 96K

# toolchain
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CPP  = $(TRGT)g++
CP   = $(TRGT)objcopy
AR	 = $(TRGT)ar -cr 
AS   = $(TRGT)gcc -x assembler-with-cpp
HEX  = $(CP) -O ihex
REMOVE = rm -f
REMOVEDIR = rm -rf
PYTHON := "py.exe"
IAP  := $(PYTHON) "iap.py"
PYOCD = "pyocd"

# List all default C defines here, like -D_DEBUG=1
DDEFS = -D$(MCU)
DDEFS += -DUSE_HAL_DRIVER
DDEFS += -DHSE_VALUE=$(FREQ)
#DDEFS += -DARM_MATH_CM0
DDEFS += -D LV_CONF_PATH=\"lv_conf.h\"
DLIBS = -lstdc++ -fno-use-cxa-atexit
 
PROJECT = app

# List source files here
HALDIR = d:/work/libs/cmsis/hal_f1
CORELIBDIR = $(HALDIR)/_cmsis
HALDRV_DIR = $(HALDIR)/_f1xx_hal_drv
HALDRV_SRCDIR = $(HALDRV_DIR)/src
HALDRV_INCDIR = $(HALDRV_DIR)/inc
STARTUPDIR = $(HALDIR)/_startup

## List ASM source files here
ASRC = $(STARTUPDIR)/startup_stm32f103xe.s

## List C source files here

#SRC = $(wildcard *.c _libs/*.c app/*.c drv/*.c usb/*.c bsp/*.c thirdparty/*/*.c)
SRCC = $(wildcard $(patsubst %,%/*.cc, . _libs app bsp drv port ui thirdparty/*))
SRC =  $(wildcard $(patsubst %,%/*.c,  . _libs app bsp drv port ui thirdparty/*))

SRC_HALDRV = $(wildcard $(HALDRV_SRCDIR)/*.c)
OBJS_HALDRV = $(SRC_HALDRV:.c=.o) 

LVGL_DIR = d:/work/3rdparty/lvgl9

SRC_LVGL = $(wildcard $(LVGL_DIR)/src/*.c) #*
SRC_LVGL += $(wildcard $(LVGL_DIR)/src/*/*.c) #*/
SRC_LVGL += $(wildcard $(LVGL_DIR)/src/*/*/*.c) #*/
SRC_LVGL += $(wildcard $(LVGL_DIR)/src/*/*/*/*.c) #*/ 
OBJS_LVGL = $(SRC_LVGL:.c=.o) 


## List all user directories here
UINCDIR = ./_libs $(CORELIBDIR) $(DEV_DIR) $(HALDRV_INCDIR)
UINCDIR += $(patsubst %,./%, bsp drv app port ui)
UINCDIR += $(patsubst %,./thirdparty/%, zuart zpin zfifo zi2c zcli ztask zkey zeep)

UINCDIR += $(patsubst %,%, $(LVGL_DIR))
UINCDIR += $(patsubst %,%, $(LVGL_DIR)/src)
UINCDIR += $(patsubst %,%, $(LVGL_DIR)/src/widgets)

# Define optimisation level here
OPT = -Os 
# End of user defines

# Define linker script file here
LDSCRIPT = ./_ldscripts/link.lds
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
CFLAGS = $(CFLAGS_COMMON) -Wstrict-prototypes -Wstrict-prototypes -Wa,-ahlms=$(<:.c=.lst) $(DEFS) -std=gnu17
CFLAGS += -Wno-maybe-uninitialized
CPPFLAGS = $(CFLAGS_COMMON) -Wa,-ahlms=$(<:.cc=.lst) $(DEFS) -std=gnu++17 -fno-exceptions -Wno-register 
LDFLAGS = $(MCFLAGS) -mthumb -T$(LDSCRIPT)
LDFLAGS += -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch $(LIBDIR)
LDFLAGS += -Wl,--gc-sections -specs=nano.specs #-u _printf_float
LDFLAGS += -L. -lstm32f103 -llvgl
 
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
	printf "FLASH:%6d/%7d,%5.1f%% (text+data)\n" $$flash $$flash_size $$flash_usage; \
	printf "SRAM: %6d/%7d,%5.1f%% (data+bss) \n" $$mem $$mem_size $$mem_usage

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
	$(CP) -O ihex $(FULL_PRJ).elf $(FULL_PRJ).hex
libstm32f103.a: $(OBJS_HALDRV)
	$(AR) $@ $(OBJS_HALDRV)
	@echo $@ done. 
liblvgl.a: $(OBJS_LVGL)
	@echo $(SRC_LVGL)
	@echo $(OBJS_LVGL)
	$(AR) $@ $(OBJS_LVGL)
	@echo $@ done.  
flash:
	$(PYOCD) flash -t gd32f303cg -f 500k $(FULL_PRJ).hex
reset:
	$(PYOCD) reset -t gd32f303cg
erase:
	$(PYOCD) erase -c -t gd32f303cg
clean:
	$(REMOVE) $(OBJS)
	rm $(FULL_PRJ).elf $(FULL_PRJ).map $(FULL_PRJ).hex $(LDSCRIPT)
	rm $(SRC:.c=.lst) $(ASRC:.s=.lst)
iap:
	$(PYTHON) zboot.py -T ihex app.hex