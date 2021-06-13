#ifndef INIT_ARM_STATE
#define INIT_ARM_STATE

/*
 * initialize `ArmState` to all 0
 *
 * - if running out of memory, it will print error and exit
 */
extern ArmState init_state(void);
extern void free_state(ArmState state);

#endif //INIT_ARM_STATE
