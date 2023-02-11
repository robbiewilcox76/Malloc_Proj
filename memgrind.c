#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"

int main(int argc, char **argv){
    char* x = malloc(4093);
    printf("%s\n", x);
    return EXIT_SUCCESS;
}