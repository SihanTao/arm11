CC = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic
CFLAGS += -Wall -Wextra -Wpedantic \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs

OBJ = ../../obj

all: file_loader.o unit_test.o bit_operation.o

%.o: %.c
	$(CC) $(CFLAGS) -o $(OBJ)/$@ -c $<