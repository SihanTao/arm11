#ifndef EXECUTE_HELPER
#define EXECUTE_HELPER

uint32_t rotate(uint32_t target, int rotate_amount);
uint32_t shift(uint32_t target, int shift_amount, shift_type type);
uint32_t arith_right(uint32_t shift_amount, uint32_t target);
bool test_instruction_cond(instruction_t instruction, ArmState arm_state);
uint32_t reg_imm_handle(bitfield * reg, reg_or_imm_t shift_or_imm, bool is_imm);

#endif // EXECUTE_HELPER
