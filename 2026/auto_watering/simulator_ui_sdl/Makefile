CC 	= gcc.exe
CPP	= g++.exe
AR	= ar.exe 
SIZE := size.exe

#LINKOBJ  = $(OBJ) $(RESOBJ)
SDL_DIR = d:/work/libs/libsdl
LVGL_DIR = d:/work/3rdparty/lvgl9

LDFLAGS =  -L. -lmingw32 -lSDL2main -lSDL2 -mwindows -lstdc++
#LDFLAGS =  -L. -lSDL2main -lSDL2 -mwindows -lstdc++
#LDFLAGS =  -L. -lSDL2main -lSDL2 #-mwindows #-lstdc++
DEFS = -DLV_CONF_INCLUDE_SIMPLE
CFLAGS_COMMON = -O2 -Wall -fno-exceptions
CFLAGS_COMMON += -I $(SDL_DIR)/include 

CFLAGS = $(CFLAGS_COMMON) -std=gnu17
CPPFLAGS = $(CFLAGS_COMMON) -std=gnu++17

LDFLAGS += -L$(SDL_DIR)/lib -L. -llvgl_mingw -lws2_32
LDFLAGS += -Wl,-Map=main.map,--cref,--no-warn-mismatch

BIN  = main.exe
RM = rm -f 
SRC = $(wildcard $(patsubst %,%/*.c, . app port ui widgets udp_server thirdparty/*))
SRCC = $(wildcard $(patsubst %,%/*.cc, . app port ui widgets udp_server thirdparty/*))

SRC_LVGL = $(wildcard $(LVGL_DIR)/src/*.c) #*
SRC_LVGL += $(wildcard $(LVGL_DIR)/src/*/*.c) #*/
SRC_LVGL += $(wildcard $(LVGL_DIR)/src/*/*/*.c) #*/
SRC_LVGL += $(wildcard $(LVGL_DIR)/src/*/*/*/*.c) #*/ 
OBJS_LVGL = $(SRC_LVGL:.c=.o)

UINCDIR += $(patsubst %,./%, app port ui udp_server widgets )
UINCDIR += $(patsubst %,./thirdparty/%, zcli zuart zfifo ztask)

UINCDIR += $(patsubst %,%, $(LVGL_DIR))
UINCDIR += $(patsubst %,%, $(LVGL_DIR)/src)
UINCDIR += $(patsubst %,%, $(LVGL_DIR)/src/widgets) 

INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))

OBJS    = $(ASRC:.s=.o) $(SRCC:.cc=.o) $(SRC:.c=.o)

all: $(BIN)
#	@$(SIZE) -B $(BIN) 
%.o: %.c
	$(CC) -c $(CFLAGS) $(DEFS) -I . $(INCDIR) $< -o $@ 
%.o: %.cc
	$(CPP) -c $(CPPFLAGS) $(CPPFLAGS) $(DEFS) -I . $(INCDIR) $< -o $@ 
clean: clean-custom
	${RM} $(LINKOBJ) $(BIN) *~ .*.swp 
$(BIN): $(OBJS)
	$(CPP) -o $(BIN) $(OBJS) $(LDFLAGS)
	strip.exe $(BIN)
#	upx.exe $(BIN) > /dev/nul 
liblvgl_mingw.a: $(OBJS_LVGL)
	@echo $(SRC_LVGL)
	@echo $(OBJS_LVGL)
	$(AR) -cr $@ $(OBJS_LVGL)
	@echo $@ done. 
.PHONY: all clean 
