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
  int        line_num;
  operand_t *operands;
} token_t;

typedef token_t *TokenStream;

// returns if it is the end of token stream
//extern bool is_end(token_t current);
extern char* get_opcode(token_t* token, char* instruction);
extern int comma_count(char* rest);
extern char** split_operand_field(char* rest, int length);
extern token_t* tokenize_instruction(char* instruction, int num_line);
void set_token_operand(token_t* token, char** operand_field, int length);
void print_token(token_t* token);
#endif // TOKENSTREAM
