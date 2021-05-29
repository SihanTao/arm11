#ifndef TYPES_AND_MACROS
#define TYPES_AND_MACROS

typedef char byte;

//the struct of byte representation in memory
typedef struct
{
    unsigned int byte4: 8;
    unsigned int byte3: 8;
    unsigned int byte2: 8;
    unsigned int byte1: 8;
} bitfield;

typedef struct arm_state_struct{
  size_t pc;
  bitfield * reg;
  byte * memory;
  bool flagN;
  bool flagZ;
  bool flagC;
  bool flagV;
} arm_state_struct;

typedef arm_state_struct * ArmState;

enum instruction_kind
{
    UNDEFINED,
    DATA_PROCESS,
    MUL,
    TRANS,
    BRANCH
};

typedef struct
{
    enum instruction_kind tag;
    union
    {
        int i;
        bitfield bf;
    } u;
} instruction_t;

#endif //TYPES_AND_MACROS
