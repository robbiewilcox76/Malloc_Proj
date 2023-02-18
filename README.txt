Sean Patrick (smp429) & Fulton Wilcox (frw14)

CS214 Systems Programming Project I: My little malloc()

Implementation Description:

The memory array is uninitialized at first, with a specific byte pattern placed in the first 4 bytes which is "1, 2, 42, 69".
This byte pattern is used as a flag to determine if memory needs to be initialzed.
Upon the first call to malloc(), we check if the first 4 bytes match that pattern, and if it does then we initialize all bytes in the array to 0
Then we allocate a short integer to represent the size of the subsequent memory chunk (metadata) which takes up 2 bytes of space, 
as opposed to other integer types that take up more space
During the very first malloc() call, a short integer will be allocated in the first 2 bytes of the array which will represent the size of the subsequent memory chunk, 
which will be 4094 bytes at first.
For every subsequent memory allocation, 2 bytes will be reserved to store the metadata in addition to however many bytes the client requests.
To determine if a chunk of memory is in use or not, we will represent the 2 byte (short) metadata as positive (free) or negative (in-use). 
This strategy utilizes less space than allocating an additional byte or more, for example, to represent whether the chunk is free or not. 
This way we minimize the amount of extra memory needed for metadata simply by changing the sign of the short integer that represents it.

We have modularized our mymalloc library with various helper functions in order to minimize code repetition and to ensure a user-friendly, 
descriptive, and readable library that can be easily interpreted. 
We have also thoroughly commented our implementation to make it easier to understand exactly how it works.

Helper functions:
void insertMetaData(void *memPtr, short chunkSize);
  -takes void pointer to metadata in memory array and short int representing chunk size parameters, casts the pointer to a short pointer, 
  and then inserts the short int chunk size at the address of the 2 bytes
short getChunkSize(void *memPtr);
  -takes void pointer to metadata in memory array parameter, casts to short int, then reads the 2 bytes at the address as a short int, and returns that short int
bool isChunkFree(void *memPtr);
  -takes void pointer to metadata in memory array parameter, reads data at address as short int, returns true if short int is positive (chunk is free)
  or false if short int is negative (chunk is in-use)
void initializeMemory();
  -initializes all bytes in the memory array to 0 during the very first call to malloc(), and inserts appropriate metadata in the first 2 bytes
void *getNextChunk(void *memPtr);
  -takes void pointer to a chunk's metadata in memory array parameter, then copies that address to another pointer,
  then adds the current chunk size + sizeof(short) to the pointer, then returns the pointer which will contain address of the next chunk's metadata
bool validPointer(void *memPtr);
  -takes void pointer to some address parameter, then checks if the address is within the bounds of the addresses of each byte of the memory array,
  returns true if it is and false otherwise
bool completePointer(void *memPtr);
  -takes void pointer to some address parameter, then checks if the pointer is the correct address which will be the first byte of the payload of the memory chunk
  by subtracting sizeof(short) from the pointer and checking if the new pointer points to the metadata of the memory chunk 
void memError(char* file, int line, int error);
  -prints descriptive error message determined by the the integer "error" parameter, indicating what error occurred, and in which file on which line it occurred
void printChunkSizes();
  -iterates through each chunk of the memory array, printing each short integer representation of the metadata (chunk size). Was very useful for correctness testing.

Test Plan:
