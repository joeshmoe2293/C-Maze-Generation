CC=gcc
CFLAGS=-g -Wall -Wpedantic
OUT=mazegen
OBJ=heap.o mazegen.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJ)

clean:
	rm -f $(OUT)
	rm -f *.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
