#ifndef ARM11_49_SRC_ASSEMBLER_CODE_GENERATE_CODE_GENERATE_HELPER_H_
#define ARM11_49_SRC_ASSEMBLER_CODE_GENERATE_CODE_GENERATE_HELPER_H_

#include "../data_structure/symbol_table.h"

SymbolTable create_mnemonic_table();
//SymbolTable create_dpi_table();
uint32_t token_to_instruction(Token token, SymbolTable symbolTable);
void token_to_dpi(Token token, instruction_t* instruction, int opcode, SymbolTable symbol_table);
void token_to_mul(Token token, instruction_t* instruction);
void token_to_trans(Token token, instruction_t* instruction, SymbolTable symbolTable);
void parse_preindexed_trans_operand(operand_t operand, trans_t *trans);
void token_to_branch(Token token, instruction_t* instruction, SymbolTable symbolTable);
SymbolTable create_cond_table();

uint32_t encode_DP(proc_t instruction);
uint32_t encode_MUL(mul_t instruction);
uint32_t encode_TRANS(trans_t instruction);
uint32_t encode_BRANCH(branch_t instruction);

#endif //ARM11_49_SRC_ASSEMBLER_CODE_GENERATE_CODE_GENERATE_HELPER_H_
