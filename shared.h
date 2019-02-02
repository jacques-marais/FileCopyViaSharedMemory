#ifndef SHARED_H_
#define SHARED_H_

/* Size of buffer in struct */
#define BUFF_SIZE	50

/* Two things for creating a key to be used by both producer and consumer */
#define FTOK_PATH	"shared.h"
#define PROJ_ID		444

/* Names of the semaphores to be used by both producer and consumer */
#define SEM_MUTEX	"/mutex"
#define SEM_ITEMS	"/items"
#define SEM_EMPTIES	"/empties"

#define TRUE		1
#define FALSE		0


/*
 * The structure to be used in shared memory. It contains the buffer,
 * two integer counters to help with producing and consuming, and a
 * "finished" int to be used as a boolean to indicate whether producer 
 * has finished.
**/
struct shared {

	int in, out, finished; /* Finished tracks whether producer has finished */

	int buffer[BUFF_SIZE];
};

/*
 * Set up / attach the shared memory and cast to struct.
 * @param shm_handle
**/
extern void setupSharedMemory(char name[], int* shm_handle, struct shared** sharedMem);

/*
 * Set up the semaphores for mutual exclusion, itemsInBuffer, and emptiesInBuffer.
 * @param mutex - Semaphore for mutual exclusion.
 * @param itemsInBuffer - Semaphore to track number of items in the buffer.
 * @param emptiesInBuffer - Semaphore to track number of empty spaces in buffer.
**/
extern void setupSemaphores(char name[], sem_t** mutex, sem_t** itemsInBuffer, 
	sem_t** emptiesInBuffer);


#endif
