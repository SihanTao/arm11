#ifndef ARM11_49_SRC_ASSEMBLER_CODE_GENERATE_CODE_GENERATE_HELPER_H_
#define ARM11_49_SRC_ASSEMBLER_CODE_GENERATE_CODE_GENERATE_HELPER_H_

#include "../data_structure/symbol_table.h"

typedef enum {
	ANDEQ,
	LSL,
	ADD,
	SUB,
	RSB,
	AND,
	EOR,
	ORR,
	MOV,
	TST,
	TEQ,
	CMP,
	MUL,
	MLA,
	LDR,
	STR,
	BEQ,
	BNE,
	BGE,
	BLT,
	BGT,
	BLE,
	B
} Mnemonic;

SymbolTable create_mnemonic_table();
instruction_t *token_to_instruction(Token token)

#endif //ARM11_49_SRC_ASSEMBLER_CODE_GENERATE_CODE_GENERATE_HELPER_H_
