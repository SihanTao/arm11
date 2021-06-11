#ifndef MNEMONIC
#define MNEMONIC

typedef enum pd_mnemonic
{
  ADD,
  SUB,
  RSB,
  AND,
  EOR,
  ORR,
  MOV,
  TST,
  TEQ,
  CMP
} pd_mnemonic;

typedef enum mul_mnemonic
{
  MUL,
  MLA
} mul_mnemonic;

typedef enum trans_mnemonic
{
  LDR,
  STR
} trans_mnemonic;

typedef enum branch_mnemonic
{
  BEQ,
  BNE,
  BGE,
  BLT,
  BGT,
  BLE,
  B
} branch_mnemonic;

typedef enum special_mnemonic
{
  LSL,
  ANDEQ
} special_mnemonic;

#endif // MNEMONIC
