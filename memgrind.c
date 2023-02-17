#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
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

void task3(){ //randomly choose between allocating 1 byte or freeing 1 byte if possible, until 120 objects have been allocated
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

void task4(){ //allocate all of memory in 30 byte chunks then free them all and request a larger chunk
    char *pointers[128];
    for(int i = 0; i < 128; i ++){
        pointers[i] = malloc(30);
    }
    for(int i = 0; i < 128; i++){
        free(pointers[i]);
    }
    char *x = malloc(62);
    free(x);
}

void task5(){ //randomly choose between allocating variable size chunk or freeing chunk if possible until 64 objects have been allocated
    char *pointers[64];
    int mallocCount = 0;
    int freeCount = 0;
    while(mallocCount != 64){
        int x = rand() % 2;
        if(x == 0){
            pointers[mallocCount++] = malloc((rand() % 62) + 1);  
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
}

void calculateTime(struct timeval start, struct timeval end, double currentTimes[], int pos, int task){
    gettimeofday(&start, NULL);
    switch(task){
        case 1:
            task1();
            break;
        case 2:
            task2();
            break;
        case 3:
            task3();
            break;
        case 4:
            task4();
            break;
        case 5:
            task5();
            break;
        default:
            task1();
            break;
    }
    gettimeofday(&end, NULL);
    currentTimes[pos] = ((end.tv_sec * 1000000 + end.tv_usec * 1000000) - (start.tv_sec * 1000000 + start.tv_usec * 1000000));
}

int main(int argc, char **argv){
    double totalTimes[5] = {0, 0, 0, 0, 0};
    double currentTimes[5] = {0, 0, 0, 0, 0};
    struct timeval start, end;
    for(int i = 0; i < 50; i ++){
        printf("Iteration %d:\n", i+1);
        for(int k = 0; k < 5; k ++){
            calculateTime(start, end, currentTimes, k, k + 1);
        }
        for(int j = 0; j < 5; j ++){
            totalTimes[j] += currentTimes[j];
            printf("Time spent on task %d: %lf microseconds\n", j+1, currentTimes[j]);
        }
        printf("\n");
    }
    for(int i = 0; i < 5; i ++){
        printf("Average time spent on task %d: %lf microseconds\n", i+1, (totalTimes[i]/50));
    }
    return EXIT_SUCCESS;
}