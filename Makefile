all: memgrind
	
memgrind: memgrind.c
	gcc -g -Wall -Werror mymalloc.c memgrind.c -o memgrind -lm
	
clean:
	rm -rf memgrind
