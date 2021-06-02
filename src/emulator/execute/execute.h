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
    EQ = 0, NE, GE = 10, LT, GT, LE, AL
};

enum {
    AND, EOR, SUB, RSB, ADD, TST, TEQ, CMP, ORR, MOV
};

enum data_processing_shift_type{
  LOGICAL_LEFT,
  LOGICAL_RIGHT,
  ARITH_RIGHT,
  ROTATE_RIGHT
};

enum {
    LOAD, STORE
};

enum {
  CONTINUE,
  EXIT,
  ERROR
};

#endif // EXECUTE
