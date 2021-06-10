#ifndef TOKENSTREAM
#define TOKENSTREAM

typedef enum operand_type
{
  NUMBER,
  STRING,
  END_OPERANDS
} operand_type;

typedef struct token_t
{
  char *     opcode;
  int        line_num;
  operand_t *operands;
} token_t;

typedef struct operand_t
{
  operand_type tag;
  union
  {
    char *letters;
    int   number;
  } operand_data;
} operand_t;

typedef token_t *TokenStream;

// returns if it is the end of token stream
extern bool is_end(token_t current);

#endif // TOKENSTREAM
