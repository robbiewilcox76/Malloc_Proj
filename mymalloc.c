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
        //printf("%d", chunkSize);
        insertMetaData(metaDataPtr, chunkSize);
    }
}

void *firstMetaData(){
    char *memPtr = memory;
    return memPtr; //returns address of first memory chunk's metadata
}

void *nextMetaData(void *memPtr){ //returns address of next chunk's metadata given a starting chunk, NULL if there are no more chunks
    char *nextChunkMetaData = memPtr;
    char *memEnd = memory + MEMSIZE;
    short chunkSize = getChunkSize(memPtr);
    if(chunkSize < 0){
        chunkSize *= -1;
    }
    nextChunkMetaData += chunkSize + sizeof(short);
    if(memEnd > nextChunkMetaData){
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

void *mymalloc(size_t size, char *file, int line){
    initializeMemory();
    return NULL;
}

void myfree(void *ptr, char *file, int line){

}

int main(int argc, char **argv){
    //initializeMemory();
    return EXIT_SUCCESS;
}
