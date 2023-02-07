all: mymalloc memgrind

mymalloc: mymalloc.c
	gcc -g -Wall -Werror mymalloc.c -o mymalloc -lm
	
memgrind: memgrind.c
	gcc -g -Wall -Werror memgrind.c -o memgrind -lm
	
clean:
	rm -rf mymalloc memgrind
