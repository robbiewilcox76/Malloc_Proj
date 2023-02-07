all: mymalloc memgrind

mymalloc: mymalloc.c
	gcc -g -Wall -Werror -fsanitize=address mymalloc.c -o mymalloc -lm
	
memgrind: memgrind.c
	gcc -g -Wall -Werror -fsanitize=address memgrind.c -o memgrind -lm
	
clean:
	rm -rf mymalloc memgrind
