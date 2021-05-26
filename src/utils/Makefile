CC	=	gcc
CFLAGS	= -g -Wall -std=c99
BUILD	= testinstructions

all:	$(BUILD)

clean:
	rm -f testinstructions *.o

testinstructions:	testinstructions.o testutils.o
testinstructions.o:	testinstructions.c instruction_processor.h
testutils.o:		testutils.c testutils.h
