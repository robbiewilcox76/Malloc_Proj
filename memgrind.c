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
    for(int i = freeCount; i != mallocCount; i++){
        free(pointers[i]);
   }
}

void task4(){
    char* x;
    char* y;
    for(int i=0; i<400; i++) {
        int num = (rand() % (2000)) + 10;
        x = malloc(num);
        y = malloc(num);
        free(x);
        free(y);
    }
}

void task5(){ //need to design our own test

}

int main(int argc, char **argv){
    task1();
    task2();
    task3();
    task4();
    //char* x = malloc(11);
    //char* y = malloc(11);
    //free(y);
    //char* z = malloc(10);
    //free(x);
    //free(z);
    /*char *x = malloc(7);
    //printMemory();
    strcpy(x, "helloo");
    printf("%s\n", x);
    free(x);
    char *y = malloc(2);
    strcpy(y, "h");
    printf("%s\n", y);
    //printMemory();
    free(y);
    //printMemory();
    // printMemory();
    char *z = malloc(2); 
    strcpy(z, "z");
    printf("%s", z);
    free(z);*/
    return EXIT_SUCCESS;
}