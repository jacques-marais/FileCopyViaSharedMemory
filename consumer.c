#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include "shared.h"
#include "consumer.h"

struct shared* sharedMem;
FILE* destinationFile;

sem_t* mutex; /* Semaphore for mutual exclusion */
sem_t* itemsInBuffer; /* Semaphore for consumer to know there are things in buffer */
sem_t* emptiesInBuffer; /* Semaphore for producer to know there are spaces in buffer */

/* 
 * Main function opens output file, runs consumer, and closes file. 
**/
int main (int argc, char* argv[])
{
	char* filename;

	if (argc != 2) {
		printf("Usage: %s output_file_name\n", argv[0]);
		exit(0);
	}

	filename = argv[1];

	/* Try to open the given file */
	openOutputFile(filename);

	/* Run the consumer which will take from buffer and write to file */
	consumer();

	/* Close output file */
	fclose(destinationFile);
}


void consumer()
{
	int shm_handle; /* Handle for shared memory */
	int temp; /* To hold the character being consumed */
	char name[] = "Consumer"; /* Name for passing to setup functions. */

	/* Set up the shared memory... */
	setupSharedMemory(name, &shm_handle, &sharedMem);

	/* Set up the semaphores... */
	setupSemaphores(name, &mutex, &itemsInBuffer, &emptiesInBuffer);

	/* Start at 0 index in the buffer */
	sharedMem->out = 0;

	/* Loop runs until end of file is reached */
	for ( ;; ) {

		sem_wait(itemsInBuffer); /* Wait for items in buffer */
		sem_wait(mutex); /* Wait for mutual exclusion */

		temp = take(); /* Take item from buffer */

		sem_post(mutex); /* Exit critical section */
		sem_post(emptiesInBuffer); /* Add to "empty spots" semaphore */

		/* Loop runs until producer is finished and the EOF marker
		 * is reached in the buffer */
		if (sharedMem->finished == TRUE && temp == EOF)
			break;

		/* Write the character to the output file */
		appendToFile(temp);
	}
	
	/* CLose down semaphores and shared memory. */
	finishConsumer(shm_handle);
}


void finishConsumer(int shm_handle)
{
	/* Close the semaphores */
	sem_close(mutex);
	sem_close(itemsInBuffer);
	sem_close(emptiesInBuffer);

	/* Unlink the semaphores because we are totally done with them */
	sem_unlink(SEM_MUTEX);
	sem_unlink(SEM_ITEMS);
	sem_unlink(SEM_EMPTIES);

	/* Detach from the shared memory segment */
	shmdt(sharedMem);

	/* Remove the shared memory segment */
	shmctl(shm_handle, IPC_RMID, 0);
}


int take()
{
	int temp;

	/* Get the next item from the buffer */
	temp = sharedMem->buffer[sharedMem->out];

	/* Cycle to the next spot in the buffer */
	sharedMem->out = (sharedMem->out + 1) % BUFF_SIZE;

	return temp;
}


void openOutputFile(char* filename)
{
	/* Open file in binary write mode */
	destinationFile = fopen(filename, "wb");

	/* If failed, display error and exit */
	if (destinationFile == NULL) {

		printf("Consumer: can't open output file");
		exit(0);
	}
}


void appendToFile(int temp)
{
	fputc(temp, destinationFile);
}
