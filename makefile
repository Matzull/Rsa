CC = gcc
CFLAGS = -Ofast -pthread -Wall -g -lgmp -lm

main: main.c
	$(CC) main.c $(CFLAGS) -o fermat

run: main
	./fermat

clean:
	rm -f *.o fermat
