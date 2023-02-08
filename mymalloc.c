#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mymalloc.h"

#define MEMSIZE 4096
#define INITFLAG 7421

static char memory[MEMSIZE];

void insertMetaData(void *memPtr, short chunkSize){
    short *metaData = (short *) memPtr;
    *metaData = chunkSize;
}

short getChunkSize(void *memPtr){
    short *chunkSize = (short *) memPtr;
    return *chunkSize;
}

bool isChunkFree(void *memPtr){
    if(getChunkSize(memPtr) < 0){
        return true;
    }
    return false;
}

void initializeMemory(){
    short *memPtr = (short *) memory;
    if(*memPtr != INITFLAG){
        memset((void *)memory, 0, MEMSIZE);
        *memPtr = INITFLAG; //using a flag to determine if the memory array has been initialized before using the memory
        char *metaDataPtr = (char *) memory + sizeof(short);
        char *memEndPtr = (char *) memory + MEMSIZE;
        short chunkSize = (memEndPtr - metaDataPtr - sizeof(short));
        //printf("%d", chunkSize);
        insertMetaData((void *)metaDataPtr, chunkSize);
    }
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
