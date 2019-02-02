#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include "shared.h"

void setupSharedMemory(char name[], int* shm_handle, struct shared** sharedMem)
{
	key_t key; /* Key for shared memory */

	/* Get the key */
	key = ftok(FTOK_PATH, PROJ_ID);

	/* Check that the key is valid */
	if (key == (key_t) -1) {

		printf("%s: ftok() failed\n", name);
		exit(0);
	}

	/* Create shared memory */
	*shm_handle = shmget(key, sizeof(struct shared), IPC_CREAT | 0666);

	/* Check that the shared memory was created/already created, or failed */
	if (*shm_handle == -1) {

		printf("%s: shared memory creation failed\n", name);
		exit(0);
	}

	/* Map the shared memory into this process's address space */
	*sharedMem = (struct shared*)shmat(*shm_handle, 0, 0);

	/* Check that the shared memory was mapped to this process's address space */
	if (*sharedMem == (struct shared*) -1) {

		printf("%s: shared memory attach failed\n", name);
		exit(0);
	}
}

void setupSemaphores(char name[], sem_t** mutex, sem_t** itemsInBuffer, 
	sem_t** emptiesInBuffer)
{
	/* Semaphore for mutual exclusion */
	*mutex = sem_open(SEM_MUTEX, O_CREAT, S_IRUSR | S_IWUSR, 1);

	/* Semaphore for consumer to know that there are things in buffer */
	*itemsInBuffer = sem_open(SEM_ITEMS, O_CREAT, S_IRUSR | S_IWUSR, 0);

	/* Semaphore for producer to know that there are spaces in buffer */
	*emptiesInBuffer = sem_open(SEM_EMPTIES, O_CREAT, S_IRUSR | S_IWUSR, BUFF_SIZE);

	if (*mutex == SEM_FAILED || *itemsInBuffer == SEM_FAILED || 
		*emptiesInBuffer == SEM_FAILED) {
		
		printf("%s: opening of semaphores failed\n", name);
		exit(0);
	}
}
