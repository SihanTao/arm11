#ifndef CODE_GENERATE
#define CODE_GENERATE

void code_generate(char * file_name, TokenStream token_stream, SymbolTable symbol_table, int* end_address);
uint32_t to_bcode(instruction_t ins);
uint32_t encode_DP(instruction_t instruction);
uint32_t encode_MUL(instruction_t instruction);
uint32_t encode_TRANS(instruction_t instruction);
uint32_t encode_BRANCH(instruction_t instruction);
void reg_imm_helper(bool is_imm, reg_or_imm_t reg_or_imm, uint32_t *target);

#endif //CODE_GENERATE
