#ifndef CONSUMER_H_
#define CONSUMER_H_

/*
 * Wait for mutual exclusion, take item from buffer, write to file,
 * and so on.
**/
extern void consumer();

/* 
 * Get the next item from the buffer.
 * @return the character (as an int)
**/
extern int take();

/*
 * Open the output file.
 * @param filename - The name of the output file.
**/
extern void openOutputFile(char* filename);

/*
 * Write to the output file.
 * @param temp - The int (character) to write to file.
*/
extern void appendToFile(int temp);

/*
 * Close down semaphores and shared memory.
 * @param shm_handle - The shared memory handle.
*/
extern void finishConsumer(int shm_handle);

#endif
