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
} operand_t;

typedef struct token_t
{
  char *     opcode;
  int        real_address;
  int        num_operand;
  operand_t *operands;
  token_t *  next;
} token_t;

typedef token_t *Token;

/*
 * token_t**
 *
 * */
typedef struct token_stream_t
{
  Token head;
  Token tail;
} token_stream_t;

typedef token_stream_t *TokenStream;

extern void        add_token_stream(token_t *elem, TokenStream stream);
extern TokenStream init_token_stream();
void               free_token_stream(TokenStream stream);

#endif // TOKENSTREAM
