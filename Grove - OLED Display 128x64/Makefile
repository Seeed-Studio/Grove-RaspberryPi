CC = g++
CFLAGS = -Wall -fstack-protector -D_FORTIFY_SOURCE=2 -O2 -I/usr/local/include -I. 
DEPS = 
OBJ = main.o SeeedOLED.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

OLED: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm ./*.o
	rm OLED
