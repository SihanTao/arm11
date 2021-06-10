#include <stdbool.h>
#include <string.h>

#include "../data_structure/token_stream.h"
#include "../data_structure/symbol_table.h"

#include "parse_helper.h"

void preprocess_and_gen_sym_table(char** loaded_file, TokenStream token_stream,
	SymbolTable symbol_table)
{
	char* line;
	for (int i = 0; loaded_file[i] != NULL; i++)
	{
		line = loaded_file[i];
		if (is_label_line(line))
		{
			// next line of label is the fake address
			add_symbol_table(line, i + 1, symbol_table);
		}
		else
		{
			tokenize(line, token_stream);
		}
	}
}

bool is_label_line(char* line)
{
	return line[strlen(line) - 1] == ':';
}

NotSure allocate_address(TokenStream token_stream, SymbolTable symbol_table)
{
	operand_t* label_operand_ptr;
	int fake_address;
	for (int i = 0; !is_end(token_stream[i]); i++)
	{
		label_operand_ptr = get_label_ptr(token_stream[i], symbol_table);

		if (label_operand_ptr != NULL)
		{
			fake_address = find_symbol_table(label_operand_ptr->operand_data.letters,
				symbol_table);
			label_operand_ptr->tag = NUMBER;
			free(label_operand_ptr->operand_data.letters);
			label_operand_ptr->operand_data.number = fake_address;
		}
	}
	return;
}

operand_t* get_label_ptr(token_t token, SymbolTable symbol_table)
{
	operand_t* operands = token.operands;
	for (int i = 0; operands[i].tag != END_OPERANDS; i++)
	{
		if (operands[i].tag == STRING
			&& find_symbol_table(operands[i].operand_data.letters, symbol_table))
		{
			return (operands + i);
		}
	}

	return NULL;
}