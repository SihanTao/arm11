#ifndef EXECUTE
#define EXECUTE

// execute will not receive NULL value;
extern bool execute(instruction_t* decode, ArmState armstate);
void execute_DP(instruction_t* decode, ArmState armstate);
void execute_MUL(instruction_t* decode, ArmState armstate);
void execute_SDT(instruction_t* decode, ArmState armstate);
void execute_BRANCH(instruction_t* decode, ArmState armstate);
void execute_ZERO(instruction_t* decode, ArmState armstate0);
bool test_instruction_cond(instruction_t* instruction, ArmState armstate);

enum {
    EQ = 0, NE, GE = 0b1010, LT, GT, LE, AL
};

#define EXIT true;
#define CONTINUE false;

#endif // EXECUTE
