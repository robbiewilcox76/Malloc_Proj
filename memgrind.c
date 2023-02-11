#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"

int main(int argc, char **argv){
    char* x = malloc(9);
    strcpy(x, "abc");
    char* y = malloc(6);
    char* z = malloc(4);
    char* q = malloc(12);
    char* m = malloc(63);
    free(x);
    free(y);
    free(z);
    free(q);
    free(m);
    free(x);
    int xyz = 0;
    free(&xyz);
    
    return EXIT_SUCCESS;
}