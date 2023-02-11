#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"

void task1(){
    for(int i = 0; i < 120; i ++){ //malloc and immediately free 120 1-byte chunks
        char *x = malloc(1);
        free(x);
    }
}

void task2(){
    char *pointers[120];
    for(int i = 0; i < 120; i ++){ //malloc 120 1-byte chunks, store them in array of pointers
        pointers[i] = malloc(1);
    }
    for(int i = 0; i < 120; i ++){ //free each chunk 
        free(pointers[i]);
    }
}

void task3(){
    char *pointers[120];
    int mallocCount = 0;
    int freeCount = 0;
    while(mallocCount != 120){
        int x = rand() % 2; //x = 0 or 1
        if(x == 0){
            pointers[mallocCount++] = malloc(1);
        }
        else{
            if(freeCount != mallocCount){ 
                free(pointers[freeCount++]);
            }
        }
    }
    for(int i = freeCount; freeCount != mallocCount; freeCount++){
        free(pointers[i]);
    }
}

void task4(){ //need to design our own test

}

void task5(){ //need to design our own test

}

int main(int argc, char **argv){
    //task1();
    //task2();
    //task3();
    char *x = malloc(1);
    char *y = malloc(1);
    free(x);
    char *z = malloc(1);  //why is this a problem? calling malloc here causes line 59 to fail even though the pointer is correct
    free(y);
    free(z);
    return EXIT_SUCCESS;
}