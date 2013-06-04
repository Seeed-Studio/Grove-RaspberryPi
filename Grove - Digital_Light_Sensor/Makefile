CC = g++
CFLAGS = -Wall -fstack-protector -D_FORTIFY_SOURCE=2 -O2 -I/usr/local/include -I. 
DEPS = 
OBJ = main.o Digital_Light_TSL2561.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

DLS: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm ./*.o
	rm DLS
