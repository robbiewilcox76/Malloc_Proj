Sean Patrick & Fulton Wilcox
smp429 & frw14

The memory array is uninitialized at first, with a specific byte pattern placed in the first 4 bytes which is "1, 2, 42, 69".
This byte pattern is used as a flag to determine if memory needs to be initialzed.
Upon the first call to mymalloc, we check if the first 4 bytes match that pattern, and if it does then we initialize all bytes in the array to 0
Then we allocate a short integer to represent the size of the subsequent memory chunk (metadata) which takes up 2 bytes of space, 
as opposed to other integer types that take up more space
During the very first mymalloc call, a short integer will be allocated in the first 2 bytes of the array which will represent the size of the subsequent memory chunk, 
which will be 4094 bytes at first.
For every subsequent memory allocation, 2 bytes will be reserved to store the metadata in addition to however many bytes the client requests.
To determine if a chunk of memory is in use or not, we will represent the 2 byte (short) metadata as positive (free) or negative (in-use). 
This strategy utilizes less space than allocating an additional byte or more, for example, to represent whether the chunk is free or not. 
This way we minimize the amount of extra memory needed for metadata simply by changing the sign of the short integer that represents it.
