#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mymalloc.h"

#define MEMSIZE 4096
#define INITFLAG '_'

static char memory[MEMSIZE] = {INITFLAG};

void insertMetaData(void *memPtr, short chunkSize){
    short *metaData = memPtr;
    *metaData = chunkSize;
}

short getChunkSize(void *memPtr){
    short *chunkSize = memPtr;
    //printf("%p", memPtr);
    return *chunkSize;
}

bool isChunkFree(void *memPtr){
    if(getChunkSize(memPtr) > 0){
        return true;
    }
    return false;
}

void initializeMemory(){
    char *memPtr = memory;
    if(*memPtr == INITFLAG){ //using a flag to determine if the memory array has been initialized before using the memory
        memset(memory, 0, MEMSIZE);
        char *metaDataPtr = memory;
        char *memEndPtr = memory + MEMSIZE;
        short chunkSize = (memEndPtr - metaDataPtr - 2);
       // printf("%d", chunkSize);
        insertMetaData(metaDataPtr, chunkSize);
    }
}

void *getFirstChunk(){
    char *memPtr = memory;
    return memPtr; //returns address of first memory chunk's metadata
}

void *getNextChunk(void *memPtr){ //returns address of next chunk's metadata given a starting chunk, NULL if there are no more chunks
    char *nextChunk = memPtr;
    char *memEnd = memory + MEMSIZE;
    short chunkSize = getChunkSize(memPtr);
    if(chunkSize < 0) {
        chunkSize *= -1;
    }
    nextChunk += chunkSize + 2;
    if(memEnd > nextChunk) {
        return nextChunk;
    }
    return NULL;
}

bool validPointer(void *memPtr){ //checks if a given pointer is within the bounds of the memory array
    char *memStart = getFirstChunk();
    char *memEnd = memory + MEMSIZE;
    char *ptr = memPtr;
    if(ptr >= memStart && ptr < memEnd){
        return true;
    }
    return false;
}

bool completePointer(void *memPtr){ //checks if a given pointer is complete(If it correctly points to the beginning of the memory chunk (its metadata) and not somewhere in between)
    void *metaData = getFirstChunk();
    while(metaData != NULL){
        if(memPtr == metaData){
            return true;
        }
        metaData = getNextChunk(metaData);
    }
    return false;
}

void memError(char* file, int line, int error) {
    //0 = not enough space, 1 = double-free, 2 = bad pointer
    //printf("%s", file);
    switch(error) {
        case(0): {printf("ERROR: Not enough available space! Malloc called in %s on line %d ", file, line); return;}
        case(1): {printf("ERROR: Double free"); return;}
        case(2): {printf("ERROR: Pointer not allocated/bad pointer"); return;}
    }
}

void *mymalloc(size_t size, char *file, int line){
    initializeMemory();
    void* chunkFinder = getFirstChunk();  //pointer to first chunk
    while(chunkFinder != NULL){
        short chunkSize = getChunkSize(chunkFinder);
        if(chunkSize < 0){
            chunkSize *= -1;
        }
        if(isChunkFree(chunkFinder) && chunkSize >= size){
            break;
        }
        chunkFinder = getNextChunk(chunkFinder);
    }
    if(chunkFinder == NULL){
        memError(file, line, 0);
        return NULL;
    }
    short chunkSize = getChunkSize(chunkFinder);
    if(chunkSize < 0){
        chunkSize *= -1;
    }
    short remainingMemory = (chunkSize - (short)size - 2);
    if(remainingMemory <= 0){
        insertMetaData(chunkFinder, -(chunkSize));
        return chunkFinder;
    }
    insertMetaData(chunkFinder, -(short)size);
    insertMetaData((getNextChunk(chunkFinder)), remainingMemory);
    return chunkFinder;
}

void myfree(void *ptr, char *file, int line) {

}

int main(int argc, char **argv){
    char* x = malloc(9);
    //strcpy(x, "abc");
    char* y = malloc(6);
    char* z = malloc(4);
    char* q = malloc(12);
    char* m = malloc(63);

    //prints array
    short* ptr = (short*)memory;
    for(int i=0; i<4080; i+=1) {printf(" |%d| ", *ptr); ptr+=1;}

    //just printing them so i dont get an unused variable error
    printf("\n%d",*x);
    printf("\n%d",*y);
    printf("\n%d",*z);
    printf("\n%d",*q);
    printf("\n%d",*m);

    //   for(int i = 0; i < 4096; i ++){ //just checking if we can print every byte of the array
    //       printf("|%d| ", memory[i]);
    //   }

    // short *p = (short *)getFirstChunk(); //checking if the size of allocated chunks are correct
    // while(p != NULL){
    //     printf("\n%d", *p);
    //     p = getNextChunk(p);
    // }

    return EXIT_SUCCESS;
}
