CC=gcc
CFLAGS=-Wall -g -lwiringPi
OBJECTS=interface.o lcd.o

interface: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o interface

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf interface *.o
