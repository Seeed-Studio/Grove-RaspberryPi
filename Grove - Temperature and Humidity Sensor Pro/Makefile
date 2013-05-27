CC = gcc
CFLAGS = -std=c99 -Wall -fstack-protector -D_FORTIFY_SOURCE=2 -O2 -I/usr/local/include -I. -lwiringPi
DEPS = 
OBJ = dht22.o locking.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Seeed_DHT22: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm ./*.o
	rm Seeed_DHT22
