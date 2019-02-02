#ifndef PRODUCER_H_
#define PRODUCER_H_

/*
 * Wait for mutual exclusion, take item from file, write to buffer,
 * and so on.
**/
extern void producer();

/* 
 * Add item to buffer.
 * @param temp - The item to add.
**/
extern void append(int temp);

/* 
 * Open the input file.
 * @param filename - The name of the input file.
**/
extern void openInputFile(char* filename);

/* 
 * Read item from input file.
 * @return the int (character) that was read.
**/
extern int readFromFile();

#endif
