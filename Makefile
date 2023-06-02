build: main.c compress.c decompress.c
	gcc compress.c decompress.c main.c -lm -Wall -o cppm
clean:
	rm *.o cppm