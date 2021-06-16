#ifndef TOKENSTREAM
#define TOKENSTREAM

typedef struct token_t
{
  AST ast;
  int imm_val;
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

#endif // TOKENSTREAM
