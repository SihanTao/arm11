
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

typedef char byte;

typedef arm_state_struct * ArmState;

typedef struct
{
    enum instruction_kind tag;
    union
    {
        uint32_t i;
        bitfield bf;
    } u;
} instruction_t;

enum instruction_kind
{
    UNDEFINED,
    DATA_PROCESS,
    MUL,
    TRANS,
    BRANCH
};