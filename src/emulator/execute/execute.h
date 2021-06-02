#ifndef EXECUTE
#define EXECUTE

// // execute will not receive NULL value;
extern bool execute(instruction_t* decode, ArmState armstate);
extern void execute_MUL(instruction_t* decode, ArmState armstate);
void execute_SDT(instruction_t* decode, ArmState armstate);
void execute_BRANCH(instruction_t* decode, ArmState armstate);
void execute_ZERO(instruction_t* decode, ArmState armstate);

typedef enum exit_type{
  CONTINUE,
  EXIT,
  ERROR
} exit_type;

#endif // EXECUTE
