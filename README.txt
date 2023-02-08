Sean Patrick & Fulton Wilcox
smp429 & frw14

Memory is uninitialized at first, with a single '_' character placed at the beginning.
This character is used as a flag to determine if memory needs to be initialzed.
Upon the first call to mymalloc, we check if the '_' character exists, and if it does then we initialize all bytes to 0
Then we allocate a short integer to represent the size of the subsequent memory chunk (metadata).
After the first mymalloc call, memory will consist of 2 bytes which represent 4094, and then the remaining free 4094 bytes.
For every subsequent memory allocation, 2 bytes will be reserved to store the metadata.
To determine if a chunk of memory is in use or not, we will represent the 2 byte metadata as positive (free) or negative (allocated).