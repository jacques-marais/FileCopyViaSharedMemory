# FileCopyViaSharedMemory
Copy a file using a shared memory region, using semaphores to synchronize the producer and consumer accessing the buffer.

HOW TO USE:
1. Clone or download the directory.
2. Run "make" in the same directory as the project files.
3. Run "./producer [input file]" to start the producer.
4. Run "./consumer [output file]" to start the consumer.
5. The output file will now be the copied version of your input file.



WHAT IF IT DOESN'T WORK?
- Run through the steps above once more.
- Look at the makefile for potential ways in which you can adapt it to your own system.
