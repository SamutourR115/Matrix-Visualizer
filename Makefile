CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wmissing-prototypes -Wextra $(shell pkg-config --cflags gtk4)

LD = gcc
LDFLAGS = $(shell pkg-config --libs gtk4)

all: maxtrixCalculatrice

maxtrixCalculatrice : main.o model/function.o view/windows.o
	$(LD) $^ -o $@ $(LDFLAGS)

%.o : %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean :
	rm -f *.o model/*.o view/*.o maxtrixCalculatrice

.PHONY: all clean