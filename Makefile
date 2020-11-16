CC=gcc
CFLAGS=-g -Wall -Wpedantic
OUT=mazegen
TEST_OUT=test
OBJ=heap.o mazegen.o bitmap.o
TEST=test.o bitmap.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJ)

test: $(TEST)
	$(CC) $(CFLAGS) -o $(TEST_OUT) $(TEST)

clean:
	rm -f $(OUT)
	rm -f $(TEST_OUT)
	rm -f *.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
