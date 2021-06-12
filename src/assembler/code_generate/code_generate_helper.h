#ifndef ARM11_49_SRC_ASSEMBLER_CODE_GENERATE_CODE_GENERATE_HELPER_H_
#define ARM11_49_SRC_ASSEMBLER_CODE_GENERATE_CODE_GENERATE_HELPER_H_

#include "../data_structure/symbol_table.h"

typedef enum {
	AND,
	EOR,
	SUB,
	RSB,
	ADD,
	TST,
	TEQ,
	CMP,
	ORR,
	MOV,
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
	B,
	ANDEQ,
	LSL
} Mnemonic;

SymbolTable create_mnemonic_table();
//SymbolTable create_dpi_table();
instruction_t *token_to_instruction(Token token);
void token_to_dpi(Token token, instruction_t* instruction, int);

#endif //ARM11_49_SRC_ASSEMBLER_CODE_GENERATE_CODE_GENERATE_HELPER_H_
