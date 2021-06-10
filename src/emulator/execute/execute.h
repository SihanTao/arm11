#ifndef EXECUTE
#define EXECUTE

/*
 * dispatcher for execute, accept a deceded instruction, and update
 * arm_state according to the instruction
 */
extern exit_type execute(instruction_t decoded, ArmState arm_state);

#endif // EXECUTE
