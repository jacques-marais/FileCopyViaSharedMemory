#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include "shared.h"
#include "producer.h"

struct shared* sharedMem;
FILE* sourceFile;

sem_t* mutex; /* Semaphore for mutual exclusion */
sem_t* itemsInBuffer; /* Semaphore for consumer to know there are things in buffer */
sem_t* emptiesInBuffer; /* Semaphore for producer to know there are spaces in buffer */

/* 
 * Main function opens output file, runs producer, and closes file. 
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
	openInputFile(filename);

	/* Run the producer which will take from file and write to buffer */
	producer();

	/* Close input file */
	fclose(sourceFile);
}


void producer()
{
	int shm_handle; /* Handle for shared memory */
	int temp; /* To hold the character being produced */
	char name[] = "Producer"; /* Name for passing to setup functions. */
	
	/* Set up the shared memory... */
	setupSharedMemory(name, &shm_handle, &sharedMem);

	/* Set up the semaphores... */
	setupSemaphores(name, &mutex, &itemsInBuffer, &emptiesInBuffer);

	/* Producer is just starting, so it's not finished. */
	sharedMem->finished = FALSE;

	/* Start at 0 index in the buffer */
	sharedMem->in = 0;

	/* Loop runs until end of file reached */
	for ( ;; ) {

		/* Read item from input file */
		temp = readFromFile();

		sem_wait(emptiesInBuffer); /* Wait for space in buffer */
		sem_wait(mutex); /* Wait for mutual exclusion */

		append(temp); /* Add item to buffer */

		sem_post(mutex); /* Exit critical section */
		sem_post(itemsInBuffer); /* Add to "num items in buffer" semaphore */

		/* Loop runs until end of file reached */
		if (sharedMem->finished == TRUE)
			break;
	}

	/* Detach from the shared memory segment */
	shmdt(sharedMem);
}


void append(int temp)
{
	/* Add to buffer */
	sharedMem->buffer[sharedMem->in] = temp;

	/* Cycle to next spot in buffer */
	sharedMem->in = (sharedMem->in + 1) % BUFF_SIZE;
}


void openInputFile(char* filename)
{
	/* Open file in binary read mode */
	sourceFile = fopen(filename, "rb");

	/* If failed, display error and exit */
	if (sourceFile == NULL) {

		printf("Producer: can't open input file");
		exit(0);
	}
}


int readFromFile()
{
	int temp;

	temp = fgetc(sourceFile);

	/* When end of file is reached, set the flag. */
	if (feof(sourceFile))
		sharedMem->finished = TRUE;
	
	return temp;
}

