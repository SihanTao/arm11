#ifndef EXECUTE_HELPER
#define EXECUTE_HELPER

#define ALL_ONE (0xffffffff)

typedef struct value_carry_t
{
  int value;
  bool carry;
} value_carry_t;

value_carry_t rotate(uint32_t target, int rotate_amount);
value_carry_t shift(uint32_t target, int shift_amount, shift_type type);
/*
 * dispatcher for shift and rotate
 * value_out and carry are the output params
 * if carry is setted to NULL, this function will not change carry
 */
void reg_imm_handle(bitfield *reg, reg_or_imm_t reg_imm, bool is_imm,
                    uint32_t *value_out, bool *carry);
bool test_instruction_cond(instruction_t instruction, ArmState arm_state);
extern void execute_MUL(mul_t instruction, ArmState arm_state);
extern void execute_BRANCH(branch_t instruction, ArmState arm_state);
extern void execute_DP(proc_t instruction, ArmState arm_state);
extern void execute_TRANS(trans_t instruction, ArmState arm_state);

#endif // EXECUTE_HELPER
