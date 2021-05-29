#ifndef EXECUTE
#define EXECUTE

// execute will not receive NULL value;
extern bool execute(instruction_t* decode, ArmState armstate);
void execute_DP(instruction_t* decode, Armstate armstate);
void execute_MUL(instruction_t* decode, Armstate armstate);
void execute_SDT(instruction_t* decode, Armstate armstate);
void execute_BRANCH(instruction_t* decode, Armstate armstate);

#define EXIT true;
#define CONTINUE false;

#endif // EXECUTE