# Variables
CC = gcc
CFLAGS = -Ofast -pthread -Wall -g -lgmp -lm
# Regla principal (por defecto)
main: main.c
	$(CC) main.c $(CFLAGS) -o fermat

run: main
	./fermat

# Limpiar archivos generados
clean:
	rm -f *.o fermat
