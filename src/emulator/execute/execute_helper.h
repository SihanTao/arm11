#ifndef EXECUTE_HELPER
#define EXECUTE_HELPER

#define ALL_ONE (0xffffffff)
#define BRANCH_PAD_MASK (0xFC000000)

typedef struct value_carry_t
{
  int value;
  bool carry;
} value_carry_t;

/*
 * dispatcher for shift and rotate
 *
 * - value_out and carry are the output params
 *
 * - if carry is setted to NULL, this function will not change carry
 */
extern void reg_imm_handle(bitfield *reg, reg_or_imm_t reg_imm, bool is_imm,
                    uint32_t *value_out, bool *carry);

/*
 * - users should not call this function by default
 *
 * - rotation amount should be in range [0, 31]
 *
 * - rotation amount is not doubled in this function
 */
extern value_carry_t rotate(uint32_t target, int rotate_amount);


/*
 * - users should not call this function by default
 *
 * - shift amount should be in range [1, 31]
 */
extern value_carry_t shift(uint32_t target, int shift_amount, shift_type type);

/*
 * tells if execute this instruction or not
 */
extern bool test_instruction_cond(instruction_t instruction, ArmState arm_state);
extern void execute_mul(mul_t instruction, ArmState arm_state);
extern void execute_bran(branch_t instruction, ArmState arm_state);
extern void execute_proc(proc_t instruction, ArmState arm_state);
extern void execute_trans(trans_t instruction, ArmState arm_state);

/*
 * find the result for data processing instruction
 * `new_flag_c` is a output param, it should accept a carry bit from shift
 * barrel, and it will update this flag_c according to opcode type
 * i.e. keep the value if it is a logical operation and change it to
 * arithmetic carry out if it is a arithmetic carry out
 */
extern uint32_t dp_carried_result(pd_opcode_type opcode, uint32_t Rn,
                                  uint32_t operand2, bool *new_flag_c);

#endif // EXECUTE_HELPER
