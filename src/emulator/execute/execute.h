#ifndef EXECUTE
#define EXECUTE

// // execute will not receive NULL value;
extern exit_type execute(instruction_t decoded, ArmState armstate);
extern void execute_MUL(mul_t instruction, ArmState armstate);
extern void execute_BRANCH(branch_t instruction, ArmState armstate);



#endif // EXECUTE
