#ifndef EXECUTE_HELPER
#define EXECUTE_HELPER

#define ALL_ONE (0xffffffff)
#define BRANCH_PAD_MASK (0xFC000000)

typedef struct value_carry_t
{
  int value;
  bool carry;
} value_carry_t;


extern void reg_imm_handle(bitfield *reg, reg_or_imm_t reg_imm, bool is_imm,
                    uint32_t *value_out, bool *carry);

extern value_carry_t rotate(uint32_t target, int rotate_amount);

extern value_carry_t shift(uint32_t target, int shift_amount, shift_type type);

extern bool test_instruction_cond(instruction_t instruction, ArmState arm_state);
extern void execute_mul(mul_t instruction, ArmState arm_state);
extern void execute_bran(branch_t instruction, ArmState arm_state);
extern void execute_proc(proc_t instruction, ArmState arm_state);
extern void execute_trans(trans_t instruction, ArmState arm_state);

extern uint32_t dp_carried_result(pd_opcode_type opcode, uint32_t Rn,
                                  uint32_t operand2, bool *new_flag_c);

#endif // EXECUTE_HELPER
