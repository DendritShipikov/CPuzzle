CC = gcc
CFLAGS = -c -Wall

all: puzzle

puzzle: main.o puzzle.o
	$(CC) -o puzzle main.o puzzle.o

main.o: src/main.c src/puzzle.h
	$(CC) $(CFLAGS) src/main.c

puzzle.o: src/puzzle.c src/puzzle.h
	$(CC) $(CFLAGS) src/puzzle.c

clean:
	rm ./*.o