#ifndef EXECUTE_SDT
#define EXECUTE_SDT

void execute_SDT(instruction_t *decoded, ArmState arm_state);
uint32_t op2_carry(bitfield * reg, trans_t trans_ins);

#endif // EXECUTE_SDT