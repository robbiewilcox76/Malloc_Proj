all: mymalloc memgrind

mymalloc: mymalloc.c
	gcc -g -Wall -Werror -fsanitize=address -std=c11 mymalloc.c -o mymalloc -lm
	
memgrind: memgrind.c
	gcc -g -Wall -Werror -fsanitize=address -std=c11 memgrind.c -o memgrind -lm
	
clean:
	rm -rf mymalloc memgrind
