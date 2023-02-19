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
This also allows us to iterate through each chunk of memory like a linked list because the metadata value essentially determines where the next chunk is located.

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

mymalloc() implementation:
  First we check if the size parameter is 0 or less, and if it is then we print the appropriate error message and return NULL because it does
  not make sense to allocate 0 or less bytes.
  Then we check if the memory array needs to be initialized or not and if so, we initialize it.
  Then we iterate through each chunk of memory until we find a free chunk that is at least as large as the size requested.
  If no such chunk was found, we print the appropriate error message and return NULL.
  Then we check if the free chunk can be divided into 2 chunks, one chunk for the requested amount of memory, and another chunk after it with the remaining memory.
  If there is not enough space to split the chunks, then we insert the appropriate metadata as a negative short int to represent that it is being used,
  and then we return a pointer to the whole chunk which will contain some extra memory. We add sizeof(short) to the pointer before returning it to ensure
  that it correctly points to the payload of the chunk and not the metadata.
  If the chunk could be split, then we insert the appropriate metadata as a negative short int to indicate it is in use, then we create a new free chunk
  after the current chunk which will contain the remaining memory. Then we add sizeof(short) to the chunk pointer to ensure it points to the payload of the chunk.

myfree() implementation:
  First we check if the pointer passed to free() contains a valid address within the memory array and if it correctly points to a chunk's metadata after subtracting
  sizeof(short) from it.
  Then we check if the metadata of the chunk is a positive value because that would mean it is a free chunk.
  If any of that is true, we print the appropriate error message and exit the function.
  If not, then we make the metadata value of the appropriate chunk positive to indicate that it is free
  Then we iterate through each chunk of the memory array checking for adjacent free chunks.
  Whenever we find adjacent free chunks, we coalesce them by adding the value of the second chunk's metadata plus sizeof(short) (to account for metadata)
  to the first chunk's metadata. We do this until we have iterated through every chunk of memory, coalescing any adjacent free chunks that we find.

Test Plan:

memgrind:

test:
