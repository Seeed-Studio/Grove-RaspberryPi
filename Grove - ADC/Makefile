CC = g++
CFLAGS = -Wall -O2 -I/usr/local/include -I. 
DEPS = 
OBJ = main.o ADC121.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ADC121_DEMO: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm ./*.o
	rm ADC121_DEMO
