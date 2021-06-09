#ifndef EXECUTE_HELPER
#define EXECUTE_HELPER

#define LOWEST_BIT_MASK (1)
#define HIGHEST_BIT_MASK (0x8000000)
#define ALL_ONE (0xffffffff)

typedef struct value_carry_t
{
  int value;
  bool carry;
} value_carry_t;

value_carry_t rotate (uint32_t target, int rotate_amount);
value_carry_t shift (uint32_t target, int shift_amount, shift_type type);
value_carry_t reg_imm_handle (bitfield *reg, reg_or_imm_t reg_imm,
                              bool is_imm);
bool test_instruction_cond (instruction_t instruction, ArmState arm_state);

#endif // EXECUTE_HELPER
