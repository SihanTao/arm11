CC = gcc
CFLAGS  =  -g -std=c99  #-Werror


BUILD_BRAN	=	bran_test
OBJS_BRAN	=	bran_test.o ../../parsec/ast.o bran.o ../../parsec/parsec.o ../../parsec/char_stream.o ../data_structure/symbol_table.o
BUILD_COMPONENT	=	component_test
OBJS_COMPONET	=	component_test.o component.o ../../parsec/ast.o ../../parsec/parsec.o ../../parsec/char_stream.o
BUILD_MUL	=	mul_test
OBJS_MUL	=	mul_test.o mul.o ../../parsec/ast.o ../../parsec/parsec.o ../../parsec/char_stream.o component.o
BUILD_PROC	=	proc_test
OBJS_PROC	=	proc_test.o proc.o component.o ../../parsec/ast.o ../../parsec/parsec.o ../../parsec/char_stream.o
BUILD_TRANS	=	trans_test
OBJS_TRANS	=	trans_test.o trans.o component.o ../../parsec/parsec.o ../../parsec/char_stream.o ../../parsec/ast.o ../data_structure/token_stream.o
BUILD_LINE	=	line_test
OBJS_LINE	=	line.o line_test.o trans.o bran.o mul.o proc.o component.o ../../parsec/parsec.o ../../parsec/char_stream.o ../../parsec/ast.o ../data_structure/token_stream.o ../data_structure/symbol_table.o

clean:
	rm *.o

bran_test:	$(OBJS_BRAN)
	$(CC) $(CFLAGS) $(OBJS_BRAN) $(LDFLAGS) -o bran_test $(LDLIBS)

component_test:	$(OBJS_COMPONET)
	$(CC) $(CFLAGS) $(OBJS_COMPONET) $(LDFLAGS) -o component_test $(LDLIBS)

mul_test:	$(OBJS_MUL)
	$(CC) $(CFLAGS) $(OBJS_MUL) $(LDFLAGS) -o mul_test $(LDLIBS)

proc_test:	$(OBJS_PROC)
	$(CC) $(CFLAGS) $(OBJS_PROC) $(LDFLAGS) -o proc_test $(LDLIBS)

trans_test:	$(OBJS_TRANS)
	$(CC) $(CFLAGS) $(OBJS_TRANS) $(LDFLAGS) -o trans_test $(LDLIBS)

line_test:	$(OBJS_LINE)
	$(CC) $(CFLAGS) $(OBJS_LINE) $(LDFLAGS) -o line_test $(LDLIBS)

../data_structure/symbol_table.o: ../data_structure/symbol_table.c
../data_structure/token_stream.o: ../data_structure/token_stream.c

../../parsec/ast.o:	../../parsec/ast.c
../../parsec/parsec.o: ../../parsec/parsec.c
../../parsec/char_stream.o: ../../parsec/char_stream.c

bran_test.o:	bran_test.c ../../parsec/parsec.h ../../parsec/char_stream.h ../../parsec/ast.h component.h bran.h ../data_structure/symbol_table.h
bran.o:	bran.c ../../parsec/ast.h ../../parsec/parsec.h ../../parsec/char_stream.h component.h bran.h ../data_structure/symbol_table.h

component.o: component.h ../../parsec/ast.h ../../parsec/parsec.h ../../parsec/char_stream.h
component_test.o:	component_test.c ../../parsec/ast.h ../../parsec/parsec.h ../../parsec/char_stream.h component.h

mul_test.o:	mul_test.c mul.h ../../parsec/ast.h ../../parsec/parsec.h ../../parsec/char_stream.h component.h component.h
mul.o:	mul.c ../../parsec/ast.h ../../parsec/parsec.h ../../parsec/char_stream.h component.h component.h

proc_test.o:	proc_test.c ../../parsec/ast.h ../../parsec/parsec.h ../../parsec/char_stream.h component.h proc.h
proc.o:	proc.c ../../parsec/ast.h ../../parsec/parsec.h ../../parsec/char_stream.h component.h proc.h

trans_test.o:	trans_test.c ../../parsec/ast.h ../../parsec/parsec.h ../../parsec/char_stream.h component.h trans.h ../data_structure/token_stream.h
trans.o:	trans.c ../../parsec/ast.h ../../parsec/parsec.h ../../parsec/char_stream.h component.h ../data_structure/token_stream.h

line_test.o: line.h	line_test.c bran.h trans.h proc.h mul.h ../../parsec/ast.h ../../parsec/parsec.h ../../parsec/char_stream.h component.h trans.h ../data_structure/token_stream.h ../data_structure/symbol_table.h
line.o:	line.c bran.h proc.h mul.h trans.h ../../parsec/ast.h ../../parsec/parsec.h ../../parsec/char_stream.h component.h ../data_structure/token_stream.h ../data_structure/symbol_table.h