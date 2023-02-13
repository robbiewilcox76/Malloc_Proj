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
        insertMetaData(metaDataPtr, chunkSize);
    }
}

void *getFirstChunk(){
    return memory; //returns address of first memory chunk's metadata
}

void *getNextChunk(void *memPtr){ //returns address of next chunk's metadata given a starting chunk, NULL if there are no more chunks
    void *nextChunk = memPtr;
    void *memEnd = memory + MEMSIZE;
    short chunkSize = getChunkSize(memPtr);
    if(chunkSize < 0) {
        chunkSize *= -1;
    }
    nextChunk += (chunkSize + sizeof(short));
    if(memEnd > nextChunk) {
        return nextChunk;
    }
    return NULL;
}

bool validPointer(void *memPtr){ //checks if a given pointer is within the bounds of the memory array
    void *memStart = getFirstChunk();
    void *memEnd = memory + MEMSIZE;
    void *ptr = memPtr;
    if(ptr >= memStart && ptr < memEnd){
        return true;
    }
    return false;
}

bool completePointer(void *memPtr){ //checks if a given pointer is complete(If it correctly points to the beginning of the memory chunk (its payload) and not somewhere in between)
    void *metaData = getFirstChunk();
    while(metaData != NULL){
        if(memPtr - sizeof(short) == metaData){
            return true;
        }
        metaData = getNextChunk(metaData);
    }
    return false;
}

void memError(char* file, int line, int error) {
    //0 = not enough space, 1 = double-free, 2 = bad pointer
    //printf("%s", file);
    //printMemory();
    switch(error) {
        case(0): {printf("\nERROR: Not enough available space! Malloc called in %s on line %d\n", file, line); return;}
        case(1): {printf("\nERROR: Double free! Free called in %s on line %d\n", file, line); return;}
        case(2): {printf("\nERROR: Pointer not allocated by malloc! Free called in %s on line %d\n", file, line); return;}
        case(3): {printf("\nERROR: Bad pointer! Free called in %s on line %d\n", file, line); return;}
    }
}

void printMemory(){
    printf("\n");
    short* ptr = (short*)memory;
    for(int i=0; i<4080; i+=1) {printf(" |%d| ", (short)*ptr); ptr+=1;}
    printf("\n");
}

void printChunkSizes(){
    short *ptr = (short *)getFirstChunk();
    while(ptr != NULL){
        printf(" |%d| ", *ptr);
        ptr = getNextChunk(ptr);
    }
}

void *mymalloc(size_t size, char *file, int line){
    initializeMemory();
    void* chunkFinder = getFirstChunk(); //pointer to first chunk
    short currentChunkSize = 0; 
    while(chunkFinder != NULL){ //iterating through each memory chunk to find free chunk with enough space
        currentChunkSize = getChunkSize(chunkFinder);
        if(isChunkFree(chunkFinder) && (currentChunkSize >= size)){ //if current chunk is free and has atleast enough space
            break;
        }
        chunkFinder = getNextChunk(chunkFinder);
    }
    if(chunkFinder == NULL){
        memError(file, line, 0);
        return NULL;
    }
    short bytesRemaining = (currentChunkSize - sizeof(short) - size); 
    if(bytesRemaining <= 0){ //if there are bytes remaining but not enough to allocate another chunk
        insertMetaData(chunkFinder, -(currentChunkSize)); //negative chunk size to indicate it is in use
        return chunkFinder + sizeof(short);
    }
    //free chunk is not last chunk, therefore we need to allocate current chunk and create a new free chunk after it with the remaining amount of memory
    insertMetaData(chunkFinder, (short)-size);
    //printf("%d ", bytesRemaining);
    //printMemory();
    insertMetaData(getNextChunk(chunkFinder), bytesRemaining);
    return chunkFinder + sizeof(short); //need to return pointer to the payload of the chunk and not the metadata
}

void myfree(void *ptr, char *file, int line) {
    if(!validPointer(ptr)){ //if pointer is not from malloc
        memError(file, line, 2);
        //printf("1");
        return;
    }
    if(!completePointer(ptr)){ //if pointer doesn't point to correct position
        memError(file, line, 3);
        //printf("2");
        return;
    }
    if(isChunkFree(ptr - sizeof(short))){ //if pointer is valid, need to subtract sizeof(short) to get the metadata and determine if free or not
        memError(file, line, 1);
        //printf("3");
        return;
    }
    short chunkSize = getChunkSize(ptr - sizeof(short));
    insertMetaData(ptr - sizeof(short), -(chunkSize));
    //need to check if adjacent free chunks can coalesce
    void *currentChunk = getFirstChunk();
    void *nextChunk = getNextChunk(currentChunk);
    while(currentChunk != NULL && nextChunk != NULL){ //iterating through chunks to coalesce adjacent free chunks
        if(isChunkFree(currentChunk) && isChunkFree(nextChunk)){ //if adjacent chunks are free
            insertMetaData(currentChunk, getChunkSize(currentChunk) + getChunkSize(nextChunk) + sizeof(short)); //coalescing adjacent free chunks by combining first chunk's size with the second chunk's size plus the size of its metadata
            nextChunk = getNextChunk(currentChunk);
        }
        else{
            currentChunk = nextChunk;
            nextChunk = getNextChunk(currentChunk);
        }
    }
}
