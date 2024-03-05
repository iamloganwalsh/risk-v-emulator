CC=gcc
CFLAGS=-I.
TARGET = vm.c11
.SUFFIXES: .c .o
SRC = vm.c reguster_dump.c decoding.c instructions.c virtual_routines.c
OBJ = $(SRC:.c=.o)
all:$(TARGET)

run: ./$(TARGET)

clean: rm -f *.o *.obj $(TARGET)