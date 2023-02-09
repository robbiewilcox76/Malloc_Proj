#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mymalloc.h"

#define MEMSIZE 4096
#define INITFLAG '_'

static char memory[MEMSIZE] = {INITFLAG};

void insertMetaData(void *memPtr, short chunkSize){
    short *metaData = (short *) memPtr;
    *metaData = chunkSize;
}

short getChunkSize(void *memPtr){
    short *chunkSize = (short *) memPtr;
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
        short chunkSize = (memEndPtr - metaDataPtr - sizeof(short));
       // printf("%d", chunkSize);
        insertMetaData(metaDataPtr, chunkSize);
    }
}

void *firstMetaData(){
    char *memPtr = memory;
    return memPtr; //returns address of first memory chunk's metadata
}

void *nextMetaData(void *memPtr){ //returns address of next chunk's metadata given a starting chunk, NULL if there are no more chunks
    short *nextChunkMetaData = memPtr;
    short *memEnd = (short*)memory + MEMSIZE;
    short chunkSize = getChunkSize(memPtr);
    if(chunkSize < 0) {
        chunkSize *= -1;
    }
    nextChunkMetaData += (chunkSize) + 1;
    if(memEnd > nextChunkMetaData) {
        return nextChunkMetaData;
    }
    return NULL;
}

bool validPointer(void *memPtr){ //checks if a given pointer is within the bounds of the memory array
    char *memStart = firstMetaData();
    char *memEnd = memory + MEMSIZE;
    char *ptr = memPtr;
    if(ptr >= memStart && ptr < memEnd){
        return true;
    }
    return false;
}

bool completePointer(void *memPtr){ //checks if a given pointer is complete(If it correctly points to the beginning of the memory chunk (its metadata) and not somewhere in between)
    void *metaData = firstMetaData();
    while(metaData != NULL){
        if(memPtr == metaData){
            return true;
        }
        metaData = nextMetaData(metaData);
    }
    return false;
}

void memError(char* file, int line, int error) {
    //0 = not enough space, 1 = double-free, 2 = bad pointer
    //printf("%s", file);
    switch(error) {
        case(0): {printf("ERROR: Not enough available space"); return;}
        case(1): {printf("ERROR: Double free"); return;}
        case(2): {printf("ERROR: Pointer not allocated/bad pointer"); return;}
    }
}

void *mymalloc(size_t size, char *file, int line){
    initializeMemory();
    short* chunkFinder = firstMetaData();  //pointer to first chunk
    while(!isChunkFree(chunkFinder) || *chunkFinder < size) {
        chunkFinder = nextMetaData(chunkFinder);
        if(chunkFinder == NULL) {/*need to throw error;*/ return NULL;} 
    }
    short oldblock = *chunkFinder;
    *chunkFinder = (short)-size;
    if(*(1+chunkFinder+(size)) == 0) {
        *(1+chunkFinder+(size)) = (short)(oldblock-size-2);
    }
    return chunkFinder;
}

void myfree(void *ptr, char *file, int line) {}

int main(int argc, char **argv){
    initializeMemory();
    char* x = mymalloc(9, "hi", 3);
    char* y = mymalloc(6, "hello", 4);
    char* z = mymalloc(4, "hi", 3);
    char* q = mymalloc(12, "hello", 4);
    char* m = mymalloc(63, "hey," , 9);

    //prints array
    short* ptr = (short*)memory;
    for(int i=0; i<4080; i+=1) {printf(" |%d| ", *ptr); ptr+=1;}

    //just printing them so i dont get an unused variable error
    printf("\n%d",*x);
    printf("\n%d",*y);
    printf("\n%d",*z);
    printf("\n%d",*q);
    printf("\n%d",*m);
    return EXIT_SUCCESS;
}
