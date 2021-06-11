CC = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -pedantic #-Werror
CFLAGS += -Wall -Wextra -Wpedantic\
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs

DIR_SRC = .
DIR_OBJ = ../../../obj
DIR_TEST_BIN = ../../../test_bin

SRC = $(wildcard $(DIR_SRC)/*.c)
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))
TEST = $(patsubst %.c,${DIR_TEST_BIN}/%.test,$(notdir ${SRC}))

all: $(TEST)
	cd $(DIR_TEST_BIN) ; python3 testall

$(DIR_TEST_BIN)/unit_test_test.test: $(DIR_OBJ)/unit_test_test.o
	$(CC) -o $@ $< $(DIR_OBJ)/unit_test.o

$(DIR_TEST_BIN)/tools_test.test: $(DIR_OBJ)/tools_test.o
	$(CC) -o $@ $< $(DIR_OBJ)/unit_test.o  $(DIR_OBJ)/tools.o

$(DIR_TEST_BIN)/file_loader_test.test: $(DIR_OBJ)/file_loader_test.o
	$(CC) -o $@ $< $(DIR_OBJ)/unit_test.o $(DIR_OBJ)/file_loader.o $(DIR_OBJ)/tools.o

# $(DIR_TEST_BIN)/execute_test.test: $(DIR_OBJ)/execute_test.o
# 	$(CC) -o $@ $< $(DIR_OBJ)/unit_test.o  $(DIR_OBJ)/tools.o $(DIR_OBJ)/execute.o -lm \
# 	$(DIR_OBJ)/execute_DP.o $(DIR_OBJ)/execute_helper.o $(DIR_OBJ)/execute_SDT.o

$(DIR_TEST_BIN)/execute_dp_test.test: $(DIR_OBJ)/execute_dp_test.o
	$(CC) -o $@ $< $(DIR_OBJ)/unit_test.o  $(DIR_OBJ)/tools.o $(DIR_OBJ)/execute_helper.o \
	$(DIR_OBJ)/init_arm_state.o $(DIR_OBJ)/load_store.o

$(DIR_TEST_BIN)/execute_mul_test.test: $(DIR_OBJ)/execute_mul_test.o
	$(CC) -o $@ $< $(DIR_OBJ)/unit_test.o  $(DIR_OBJ)/tools.o $(DIR_OBJ)/execute_helper.o \
	$(DIR_OBJ)/init_arm_state.o $(DIR_OBJ)/load_store.o

$(DIR_TEST_BIN)/execute_trans_test.test: $(DIR_OBJ)/execute_trans_test.o
	$(CC) -o $@ $< $(DIR_OBJ)/unit_test.o  $(DIR_OBJ)/tools.o $(DIR_OBJ)/execute_helper.o \
	$(DIR_OBJ)/init_arm_state.o $(DIR_OBJ)/load_store.o

$(DIR_TEST_BIN)/execute_helper_test.test: $(DIR_OBJ)/execute_helper_test.o
	$(CC) -o $@ $< $(DIR_OBJ)/unit_test.o  $(DIR_OBJ)/tools.o $(DIR_OBJ)/execute_helper.o \
	$(DIR_OBJ)/init_arm_state.o $(DIR_OBJ)/load_store.o

$(DIR_TEST_BIN)/output_test.test: $(DIR_OBJ)/output_test.o
	$(CC) -o $@ $< $(DIR_OBJ)/unit_test.o  $(DIR_OBJ)/tools.o  $(DIR_OBJ)/output.o \
	$(DIR_OBJ)/init_arm_state.o $(DIR_OBJ)/load_store.o

$(DIR_TEST_BIN)/decode_test.test: $(DIR_OBJ)/decode_test.o
	$(CC) -o $@ $< $(DIR_OBJ)/unit_test.o  $(DIR_OBJ)/tools.o $(DIR_OBJ)/decode.o


${DIR_OBJ}/%.o:${DIR_SRC}/%.c
	$(CC) $(CFLAGS) -c  $< -o $@
