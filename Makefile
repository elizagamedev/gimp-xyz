# Predefined constants

#Linux
CC		= gcc
TARGET		= file-xyz
LDPKGLIB	= --libs
LFLAGS		=

#Windows (cross-compile from Linux)
#CC		= x86_64-w64-mingw32-gcc
#CC		= i686-w64-mingw32-gcc
#TARGET		= file-xyz.exe
#LDPKGLIB	= --libs
#LFLAGS		= -mwindows

#OS X
#CC		= gcc
#TARGET		= file-xyz
#LDPKGLIB	= --libs-only-l
#LFLAGS		= -L/tmp/skl/Gimp.app/Contents/Resources/lib

#x86
#CFLAGS		+= -m32
#LFLAGS		+= -m32

#Platform-inspecific
SRC_DIR		= src
OBJ_DIR		= obj
CFLAGS		+= $(shell pkg-config --cflags gtk+-2.0) \
		   $(shell pkg-config --cflags gimp-2.0) \
		   -O3

LFLAGS		+= $(shell pkg-config $(LDPKGLIB) glib-2.0) \
		   $(shell pkg-config $(LDPKGLIB) gtk+-2.0) \
		   $(shell pkg-config $(LDPKGLIB) gimp-2.0) \
		   $(shell pkg-config $(LDPKGLIB) gimpui-2.0) \
		   -lz

# File definitions
SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES=$(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(TARGET): $(OBJ_DIR) $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(TARGET) $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR):
	test -d $(OBJ_DIR) || mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)

install:
	mkdir -p $(DESTDIR)$(exec_prefix)/lib/gimp/2.0/plug-ins
	cp $(TARGET) $(DESTDIR)$(exec_prefix)/lib/gimp/2.0/plug-ins

uninstall:
	rm $(DESTDIR)$(exec_prefix)/lib/gimp/2.0/plug-ins/$(TARGET)

.PHONY: clean install uninstall
