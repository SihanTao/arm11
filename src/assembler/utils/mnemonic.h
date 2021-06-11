

typedef enum pd_mnemonic
{
  add,
  sub,
  rsb,
  AND,
  eor,
  orr,
  mov,
  tst,
  teq,
  cmp
} pd_mnemonic;

typedef enum mul_mnemonic
{
  mul,
  mla
} mul_mnemonic;

typedef enum trans_mnemonic
{
  ldr,
  str
} trans_mnemonic;

typedef enum branch_mnemonic
{
  beq,
  bne,
  bge,
  blt,
  bgt,
  ble,
  b
} branch_mnemonic;

typedef enum special_mnemonic
{
  lsl,
  andeq
} special_mnemonic;