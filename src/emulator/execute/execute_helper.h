#ifndef EXECUTE_HELPER
#define EXECUTE_HELPER

uint32_t rotate(int rotation_amount, uint32_t content);
uint32_t shift(uint32_t Rm, int shift_val, shift_type type);
uint32_t arith_right(uint32_t shift_val, uint32_t Rm);
bool test_instruction_cond(instruction_t *instruction, ArmState arm_state)

#endif // EXECUTE_HELPER
