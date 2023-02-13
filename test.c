#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.c"

void printBytePattern(char *ptr, int id){
    printf("Object %d byte pattern:\n", id);
    for(int i = 0; i < 509; i ++){
        printf(" %d ", ptr[i]);
    }
    printf("\n");
}

int main(int argc, char **argv) {
    /*
    3. Write a program that allocates several large objects. Once allocation is complete, it fills each
    object with a distinct byte pattern (e.g., the first object is filled with 1, the second with 2,
    etc.). Finally, it checks whether each object still contains the written pattern. (That is, writing
    to one object did not overwrite any other.)
    */

    /*
    memory will have 4094 free bytes at first
    each call to malloc will reserve desired bytes plus 2 bytes for metadata. 
    we can request allocation for 8 large objects of equal size = 509 bytes, and each object will reserve 511 bytes
    511 * 8 = 4088 -> there will be 6 bytes left over in memory after 8 objects are allocated
    */

    // char *a = malloc(509);
    // char *b = malloc(509);
    // char *c = malloc(509);
    // char *d = malloc(509);
    // char *e = malloc(509);
    // char *f = malloc(509);
    // char *g = malloc(509);
    // char *h = malloc(509);
    // memset(a, 1, 509);
    // memset(b, 2, 509);
    // memset(c, 3, 509);
    // memset(d, 4, 509);
    // memset(e, 5, 509);
    // memset(f, 6, 509);
    // memset(g, 7, 509);
    // memset(h, 8, 509);
    // printBytePattern(a, 1);
    // printBytePattern(b, 2);
    // printBytePattern(c, 3);
    // printBytePattern(d, 4);
    // printBytePattern(e, 5);
    // printBytePattern(f, 6);
    // printBytePattern(g, 7);
    // printBytePattern(h, 8);

    // char *x = malloc(4085);
    // char *y = malloc(4);
    // char *z = malloc(1);
    // printChunkSizes();
    // free(x);
    // free(y);
    // free(z);
    // printChunkSizes();

    char *pointers[15]; //test with 15-20
    int mallocCount = 0;
    int freeCount = 0;
    while(mallocCount != 15){ //test with 15-20
        int x = rand() % 2;
        if(x == 0){
            pointers[mallocCount++] = malloc((rand() % 200) + 1);  
        }
        else{
            if(freeCount != mallocCount){
                free(pointers[freeCount++]);
            }
        }
    }
    for(int i = freeCount; i != mallocCount; i ++){
        free(pointers[i]);
    }
    return EXIT_SUCCESS;
}