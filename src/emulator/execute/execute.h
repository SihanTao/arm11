#ifndef EXECUTE
#define EXECUTE

// // execute will not receive NULL value;
extern exit_type execute(instruction_t decoded, ArmState armstate);
extern void execute_MUL(mul_t instruction, ArmState armstate);
extern void execute_BRANCH(branch_t instruction, ArmState armstate);
extern void execute_DP(proc_t instruction, ArmState arm_state);
extern void execute_TRANS(trans_t instruction, ArmState arm_state);

#endif // EXECUTE
