#ifndef EXECUTE_DP
#define EXECUTE_DP

void execute_DP(proc_t instruction, ArmState arm_state);
bool new_carry(bitfield *reg, reg_or_imm_t shift_or_imm, bool is_imm);
void compute_result(proc_type opcode, uint32_t operand2, uint32_t Rn, bitfield Rd);

#endif // EXECUTE_DP
