#ifndef EXECUTE_DP
#define EXECUTE_DP

void execute_DP(instruction_t *decoded, ArmState arm_state);
uint32_t op2_carry(bitfield * reg, data_process_t data_ins, bool * flagC);
uint32_t compute_result(opcode_type opcode, uint32_t operand2, uint32_t Rn);

#endif // EXECUTE_DP
