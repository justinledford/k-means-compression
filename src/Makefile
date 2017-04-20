CC=gcc
CFLAGS=-Ofast -std=c99 -lm

compress: compress.o lloyds.o image.o
	$(CC) $(CFLAGS) $^ -o $@

compress.o: compress.c
	$(CC) $(CFLAGS) -c $^

lloyds.o: lloyds.c
	$(CC) $(CFLAGS) -c $^

image.o: image.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf *.o compress
