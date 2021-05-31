#ifndef EXECUTE
#define EXECUTE

// // execute will not receive NULL value;
extern bool execute(instruction_t* decode, ArmState armstate);
extern void execute_DP(instruction_t* decode, ArmState armstate);
void execute_MUL(instruction_t* decode, ArmState armstate);
void execute_SDT(instruction_t* decode, ArmState armstate);
void execute_BRANCH(instruction_t* decode, ArmState armstate);
void execute_ZERO(instruction_t* decode, ArmState armstate);
bool test_instruction_cond(instruction_t* instruction, ArmState armstate);

enum {
    EQ = 0, NE, GE = 0b1010, LT, GT, LE, AL
};

enum {
    AND, EOR, SUB, RSB, ADD, TST, TEQ, CMP, ORR, MOV
};

enum {
    load, store
};
#define EXIT true
#define CONTINUE false

#endif // EXECUTE
