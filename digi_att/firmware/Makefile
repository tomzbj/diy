MCU = atmega168p
F_CPU = 8000000
FLASH_SIZE = 16K
MEM_SIZE = 1K
FORMAT = ihex
TARGET = main
SRC = $(wildcard *.c app/*.c bsp/*.c drv/*.c thirdparty/*/*.c) # */

OPT = s # can be [0, 1, 2, 3, s]. 
EXTRAINCDIRS = 
CDEFS = -DF_CPU=$(F_CPU)UL -std=gnu99
UINCS = $(patsubst %,./%, app bsp drv thirdparty _libs )
UINCS += $(patsubst %,./thirdparty/%, ztask zuart zss zkey zevent)
CINCS = $(patsubst %,-I%, $(UINCS))

CFLAGS = $(CDEFS) -O$(OPT) -Wall#-Wstrict-prototypes
CFLAGS += -ffunction-sections -fdata-sections -std=gnu99

ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 

#---------------- Library Options ----------------
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt
PRINTF_LIB = 
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt
SCANF_LIB = 
MATH_LIB = -L. 
LDFLAGS = $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)
LDFLAGS += -Wl,--gc-sections 
LDFLAGS += -Wl,-Map=$(TARGET).map 

# Define programs and commands.
SHELL = sh
CC = avr-gcc
OBJCOPY = avr-objcopy
SIZE = avr-size
REMOVE = rm -f
COPY = cp
WINSHELL = cmd

OBJ = $(SRC:.c=.o) $(ASRC:.S=.o) 
LST = $(SRC:.c=.lst) $(ASRC:.S=.lst) 
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CINCS) $(CFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

all: build sizeafter
build: elf hex eep 
elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep

sizeafter: elf
	$(SIZE) -B --mcu=$(MCU) $(TARGET).elf
	@arr=(`$(SIZE) -B --mcu=$(MCU) $(TARGET).elf | sed -n '2p'`); \
	let flash=($${arr[0]}+$${arr[1]}); \
	let mem=($${arr[1]}+$${arr[2]}); \
	let flash_size=$(subst K,,$(FLASH_SIZE))*1024; \
	let mem_size=$(subst K,,$(MEM_SIZE))*1024; \
	flash_usage=`echo "scale=1;($$flash*100/$$flash_size)" | bc`; \
	mem_usage=`echo "scale=1;($$mem*100/$$mem_size)" | bc`; \
	echo "Flash: $$flash / $$flash_size bytes, $$flash_usage% Full (.text + .data)"; \
	echo "SRAM:  $$mem / $$mem_size bytes, $$mem_usage% Full (.data + .bss)";
	

%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@
%.eep: %.elf
	-$(OBJCOPY) -j .eeprom -O $(FORMAT) $< $@ 

.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	$(CC) $(ALL_CFLAGS) $^ -o $@ $(LDFLAGS)
%.o : %.c
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@
%.o : %.S
	$(CC) -c $(ALL_ASFLAGS) $< -o $@
%.i : %.c
	$(CC) -E -mmcu=$(MCU) -I. $(CFLAGS) $< -o $@ 

clean:
	$(REMOVE) *.hex *.eep *.cof *.elf *.map *.o *.lst *.s *.d *~

.PHONY : all finish sizeafter build elf hex eep sym coff clean program 