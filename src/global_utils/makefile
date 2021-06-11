CC	=	gcc
CFLAGS	= 	-g -Wall
BUILD	=	tools_test
OBJS	=	tools_test.o tools.o unit_test.o

all:	$(BUILD)

clean:
	/bin/rm -f $(BUILD) $(OBJS) core a.out .nfs*

tools_test:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o tools_test $(LDLIBS)

file_loader_test.o:	tools_test.c tools.h unit_test.h
assembler_file_loader.o:	tools.c tools.h
../../emulator/utils/unit_test.o:	unit_test.c