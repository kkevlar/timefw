CC = gcc
CFLAGS = -Wall -ansi -pedantic -Wno-variadic-macros -g
MAIN = timefw
OBJS = timefw.o

all : $(MAIN)

timefw : $(OBJS) timefw.h debug.h
	$(CC) $(CFLAGS) -o timefw $(OBJS)

timefw.o : timefw.c timefw.h debug.h
	$(CC) $(CFLAGS) -c timefw.c

clean :
	rm *.o $(MAIN)
