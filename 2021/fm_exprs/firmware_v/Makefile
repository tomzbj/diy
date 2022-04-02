# $Id: Makefile 123 2013-03-05 09:28:51Z tom $
MCU = attiny13 # MCU name
F_CPU = 1200000
# Processor frequency.
FORMAT = ihex # Output format. (can be srec, ihex, binary)
TARGET = main
# Target file name (without extension).
# List C source files here. (C dependencies are automatically generated.)
SRC = $(TARGET).c 
ASRC = # List Assembler source files here.
#     0 = turn off optimization. s = optimize for size.
OPT = s # Optimization level, can be [0, 1, 2, 3, s].  
DEBUG = dwarf-2 # Debugging format.
EXTRAINCDIRS = 
CSTANDARD = -std=gnu99 # Compiler flag to set the C Standard level.
CDEFS = -DF_CPU=$(F_CPU)UL  # Place -D or -U options here
CINCS = # Place -I options here

CFLAGS = -g$(DEBUG) #---------------- Compiler Options ----------------
CFLAGS += $(CDEFS) $(CINCS)
CFLAGS += -O$(OPT)
CFLAGS += #-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -Wall #-Wstrict-prototypes
CFLAGS += -Wa,-adhlns=$(<:.c=.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)
ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 

PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min #---- Library Options -------
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt
PRINTF_LIB = 
#PRINTF_LIB = $(PRINTF_LIB_MIN)
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt
SCANF_LIB =
#SCANF_LIB = $(SCANF_LIB_MIN)
#SCANF_LIB = $(SCANF_LIB_FLOAT)

MATH_LIB = -L. #-lm
EXTMEMOPTS =

LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
#  -Wl,...:to pass this to linker.  
#  -Map: create map file --cref: add cross reference to  map file
LDFLAGS += $(EXTMEMOPTS)
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)

DUDEFLAGS = -p $(MCU) -c usbasp -qq

SHELL = sh # Define programs and commands.
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
COPY = cp
WINSHELL = cmd
UNIQ = uniq

OBJ = $(SRC:.c=.o) $(ASRC:.S=.o) # Define all object files.
LST = $(SRC:.c=.lst) $(ASRC:.S=.lst) # Define all listing files.
#GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d 
# Compiler flags to generate dependency files.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS) 
# Combine all necessary flags and optional flags.
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS) 
# Add target processor to flags.

all: gccversion build sizeafter 
	#flash
# Default target.
build: elf hex eep lss sym

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss 
sym: $(TARGET).sym

HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex # Display size of file.
ELFSIZE = $(SIZE) -B --mcu=$(MCU) $(TARGET).elf | $(UNIQ) -u
AVRMEM = avr-mem.sh $(TARGET).elf $(MCU)

sizeafter:
	@if test -f $(TARGET).elf; then $(ELFSIZE);  fi

%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

%.lss: %.elf
	$(OBJDUMP) -h -S $< > $@ # Create extended listing file from ELF output file.

%.sym: %.elf
	$(NM) -n $< > $@ # Create a symbol table from ELF output file.

.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	$(CC) $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS) 
	# Link: create ELF output file from object files.

%.o : %.c
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 
	# Compile: create object files from C source files.

%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@ 
	# Compile: create assembler files from C source files.

%.o : %.S
	$(CC) -c $(ALL_ASFLAGS) $< -o $@ 
	# Assemble: create object files from assembler source files.

%.i : %.c
	$(CC) -E -mmcu=$(MCU) -I. $(CFLAGS) $< -o $@ 
	# Create preprocessed source for use in sending a bug report.

flash:
	$(AVRDUDE) $(DUDEFLAGS) -U flash:w:$(TARGET).hex

clean: clean_list

clean_list :
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).eep
	$(REMOVE) $(TARGET).cof
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(OBJ)
	$(REMOVE) $(LST)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRC:.c=.d)
	$(REMOVE) .dep/*
	$(REMOVE) *~

#-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*) 
# Include the dependency files.

# Listing of phony targets.
.PHONY : all finish sizeafter gccversion \
build elf hex eep lss sym coff extcoff \
clean clean_list program debug gdb-config
