#ifndef INIT_ARM_STATE
#define INIT_ARM_STATE

/*
 * make some fake ArmState, all 0, for testing
 * out of memory problem is handled inside function
 */
extern ArmState init_state(void);

extern void free_state(ArmState state);

#endif //INIT_ARM_STATE
