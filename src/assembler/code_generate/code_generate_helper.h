#ifndef ARM11_49_SRC_ASSEMBLER_CODE_GENERATE_CODE_GENERATE_HELPER_H_
#define ARM11_49_SRC_ASSEMBLER_CODE_GENERATE_CODE_GENERATE_HELPER_H_

#include "../data_structure/symbol_table.h"

typedef enum {
	AND_M,
	EOR_M,
	SUB_M,
	RSB_M,
	ADD_M,
	TST_M,
	TEQ_M,
	CMP_M,
	ORR_M,
	MOV_M,
	MUL_M,
	MLA_M,
	LDR_M,
	STR_M,
	BEQ_M,
	BNE_M,
	BGE_M,
	BLT_M,
	BGT_M,
	BLE_M,
	B_M,
	ANDEQ_M,
	LSL_M
} Mnemonic;

SymbolTable create_mnemonic_table();
//SymbolTable create_dpi_table();
instruction_t *token_to_instruction(Token token, SymbolTable symbolTable);
void token_to_dpi(Token token, instruction_t* instruction, int);
void token_to_mul(Token token, instruction_t* instruction);

#endif //ARM11_49_SRC_ASSEMBLER_CODE_GENERATE_CODE_GENERATE_HELPER_H_
