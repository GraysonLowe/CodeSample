CC=gcc
INCLUDE=include/
CFLAGS = -g -O -Wall -MMD  -I$(INCLUDE) 
LIBS=-lpthread -Lbin/ -lmylib

all: pc

pc: list_mt.o pc.o item.o item.h
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

-include *.d

clean:
	/bin/rm -f *.o pc *.d
