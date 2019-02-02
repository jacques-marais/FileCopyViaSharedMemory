#
# Default rule to execute when the make command has no arguments
#
all: producer consumer

#
# shared.o: Preprocessor + compiler + assembler step which produces object files.
#
shared.o: shared.c shared.h
	gcc -c shared.c -o shared.o -Wall -pedantic



#
# producer.o: Preprocessor + compiler + assembler step which produces object files.
#
producer.o: producer.c producer.h shared.h
	gcc -c producer.c -o producer.o -Wall -pedantic

#
# producer: Linker step which produces the final executable file.
#
producer: producer.o shared.o
	gcc producer.o shared.o -o producer -Wall -pedantic -lpthread



#
# consumer.o: Preprocessor + compiler + assembler step which produces object files.
#
consumer.o: consumer.c consumer.h shared.h
	gcc -c consumer.c -o consumer.o -Wall -pedantic

#
# consumer: Linker step which produces the final executable file.
#
consumer: consumer.o shared.o
	gcc consumer.o shared.o -o consumer -Wall -pedantic -lpthread



#
# The command-line 'make clean' command executes the following
# command, which removes all files created during the build.
#
clean:
	rm -f producer consumer producer.o consumer.o shared.o

#
# Lists the "phony" rules in this file ('all' and 'clean')
#
.PHONY: all clean
