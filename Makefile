build: main.c src/compress.c src/decompress.c
	gcc src/compress.c src/decompress.c main.c -lm -Wall -o cppm
clean:
	rm *.o cppm