#ifndef TOKENSTREAM
#define TOKENSTREAM

typedef enum mnemonic_type
{
  ADD_M,
  SUB_M,
  RSB_M,
  AND_M,
  EOR_M,
  ORR_M,
  MOV_M,
  TST_M,
  TEQ_M,
  CMP_M,
  MUL_M,
  MLA_M,
  LDR_M,
  STR_M,
  // BEQ_M,
  // BNE_M,
  // BGE_M,
  // BLT_M,
  // BGT_M,
  // BLE_M,
  B_M,
  LSL_M,
  ANDEQ_M,
  CONSTANT
} mnemonic_type;




typedef enum operand_type
{
	NUMBER,
	STRING,
} operand_type;

typedef struct operand_t
{
	operand_type tag;
	union
	{
		char* letters;
		int number;
	} operand_data;
	struct operand_t* next;
} operand_t;

typedef struct proc_token_t
{
  pd_opcode_type type;
  

} proc_token_t;

typedef struct mul_token_t
{

} mul_token_t;

typedef struct trans_token_t
{

} trans_token_t;

typedef struct bran_token_t
{

} bran_token_t;

typedef struct token_t
{
  ins_type type;
  cond_type cond;
  union {
    proc_token_t proc;
    mul_token_t mul;
    trans_token_t trans;
    bran_token_t bran;
  } token_data;
}

typedef struct token_t
{
	mnemonic_type type;
  cond_type cond;
	operand_t* operands;
	int num_of_operands;
	int address;
	struct token_t* next;
} token_t;

/*
 * A Token represents a line of assembly code
 * field `char * opcode` records assembly code's mnemonic
 * field `num_operand` records number of operand of this code
 * field `operand_t * operands` is an array of operands
 * field `next` is a pointer to next token
 */
typedef token_t* Token;

typedef struct token_stream_t
{
	Token head;
	Token tail;
} token_stream_t;

/*
 * A TokenStream is linked list based queue, new token is added to tail and
 * old token is retrieved from the head.
 */
typedef token_stream_t* TokenStream;

extern void add_token_stream(token_t* elem, TokenStream stream);
extern TokenStream init_token_stream();
extern void free_token(Token token);
extern void free_token_stream(TokenStream stream);
extern void free_operands(operand_t* operand);

#endif // TOKENSTREAM
