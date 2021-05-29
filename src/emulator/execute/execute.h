#ifndef EXECUTE
#define EXECUTE

// execute will not receive NULL value;
extern bool execute(instruction_t decoded, ArmState states);

#define EXIT true;
#define CONTINUE false;

#endif // EXECUTE
