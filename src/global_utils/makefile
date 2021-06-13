CC	=	gcc
CFLAGS	= 	-g -Wall
BUILD	=	tools_test
OBJS	=	tools_test.o tools.o unit_test.o
DIR_OBJ = ../../obj

DIR_SRC = .


SRC = $(wildcard $(DIR_SRC)/*.c)
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

all:	$(BUILD)

clean:
	/bin/rm -f $(BUILD) $(OBJS) core a.out .nfs*

tools_test:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o tools_test $(LDLIBS)

compile_all: $(OBJ)

${DIR_OBJ}/%.o:${DIR_SRC}/%.c
	$(CC) $(CFLAGS) -c  $< -o $@

file_loader_test.o:	tools_test.c tools.h unit_test.h
assembler_file_loader.o:	tools.c tools.h
../../emulator/utils/unit_test.o:	unit_test.c