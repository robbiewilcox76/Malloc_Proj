#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mymalloc.h"

#define MEMSIZE 4096
#define KEY 42069

static char memory[MEMSIZE];

void insertMetaData(void *start, short chunkSize){
    short *size = (short *) start;
    *size = chunkSize;
}

short getChunkSize(void *start){
    short *chunkSize = (short *) start;
    return *chunkSize;
}

bool isChunkFree(void *start){
    if(getChunkSize(start) < 0){
        return true;
    }
    return false;
}

void initializeMemory(){
    unsigned short *start = (unsigned short *) memory;
    if(*start != KEY){
        memset((void *)memory, 0, MEMSIZE);
        *start = KEY; //using a key to determine if the memory array has been initialized before using the memory
        char *metaDataStart = (char *) memory + sizeof(unsigned short);
        char *end = (char *) memory + MEMSIZE;
        short chunkSize = -(end - metaDataStart - sizeof(short));
        //printf("%d", chunkSize);
        insertMetaData((void *)metaDataStart, chunkSize);
    }
}

void *mymalloc(size_t size, char *file, int line){

    return NULL;
}

void myfree(void *ptr, char *file, int line){

}

int main(int argc, char **argv){
    //initializeMemory();
    return EXIT_SUCCESS;
}