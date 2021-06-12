#ifndef TOKENSTREAM
#define TOKENSTREAM

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
    char *letters;
    int   number;
  } operand_data;
  struct  operand_t* next;
} operand_t;

typedef struct token_t
{
  char *     opcode;
  operand_t *operands;
  struct token_t *  next;
} token_t;

/*
 * A Token represents a line of assembly code
 * field `char * opcode` records assembly code's mnemonic
 * field `num_operand` records number of operand of this code
 * field `operand_t * operands` is an array of operands
 * field `next` is a pointer to next token
 */
typedef token_t *Token;

typedef struct token_stream_t
{
  Token head;
  Token tail;
} token_stream_t;

/*
 * A TokenStream is linked list based queue, new token is added to tail and
 * old token is retrieved from the head.
 */
typedef token_stream_t *TokenStream;

extern void        add_token_stream(token_t *elem, TokenStream stream);
extern TokenStream init_token_stream();
extern Token       pop_token_stream(TokenStream stream);
void               free_token_stream(TokenStream stream);

#endif // TOKENSTREAM
