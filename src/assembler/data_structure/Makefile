# Autogenerated by mfbuild-perl

BUILD	=	symbol_table_test
OBJS	=	symbol_table_test.o symbol_table.o ../../global_utils/unit_test.o

all:	$(BUILD)

clean:
	/bin/rm -f $(BUILD) $(OBJS) core a.out .nfs*

symbol_table_test:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o symbol_table_test $(LDLIBS)

symbol_table_test.o:	symbol_table_test.c symbol_table.h ../../global_utils/unit_test.h
symbol_table.o:	symbol_table.c symbol_table.h
../../global_utils/unit_test.o:	../../global_utils/unit_test.c 
