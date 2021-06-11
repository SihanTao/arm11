CC = gcc
CFLAGS =  -g -Wall
BUILD = helper_test
OBJS = helper_test.o helper.o ../../global_utils/unit_test.o

all: $(BUILD)

clean:
 /bin/rm -f $(BUILD) $(OBJS) core a.out .nfs*

helper_test: $(OBJS)
 $(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o helper_test $(LDLIBS)

helper_test.o: helper_test.c helper.h ../../global_utils/unit_test.h
helper.o: helper.c helper.h
../../global_utils/unit_test.o: ../../global_utils/unit_test.c