#include <stdint.h>
#include <stdbool.h>

#include "code_generate_helper.h"
#include "../data_structure/token_stream.h"
#include "../../global_utils//tools.h"
#include "../../global_utils/types_and_macros.h"
#include "mnemonic.h"
#include "reverse_rotate.h"
#include "allocate_address.h"
#include "symbol_table.h"

SymbolTable create_mnemonic_table()
{
	SymbolTable new_table = init_symbol_table();
	add_symbol_table("andeq", ANDEQ, new_table);
	add_symbol_table("lsl", LSL, new_table);
	add_symbol_table("add", ADD, new_table);
	add_symbol_table("sub", SUB, new_table);
	add_symbol_table("rsb", RSB, new_table);
	add_symbol_table("and", AND, new_table);
	add_symbol_table("eor", EOR, new_table);
	add_symbol_table("orr", ORR, new_table);
	add_symbol_table("mov", MOV, new_table);
	add_symbol_table("tst", TST, new_table);
	add_symbol_table("teq", TEQ, new_table);
	add_symbol_table("cmp", CMP, new_table);
	add_symbol_table("mul", MUL, new_table);
	add_symbol_table("mla", MLA, new_table);
	add_symbol_table("ldr", LDR, new_table);
	add_symbol_table("str", STR, new_table);
	add_symbol_table("beq", BEQ, new_table);
	add_symbol_table("bne", BNE, new_table);
	add_symbol_table("bge", BGE, new_table);
	add_symbol_table("blt", BLT, new_table);
	add_symbol_table("bgt", BGT, new_table);
	add_symbol_table("ble", BLE, new_table);
	add_symbol_table("b", B, new_table);

	return new_table;
}

uint32_t token_to_instruction(Token token, SymbolTable symbolTable)
{
	SymbolTable mnemonic_table = create_mnemonic_table();
	instruction_t* instruction = calloc(1, sizeof(instruction_t));
	int find = find_symbol_table(token->opcode, mnemonic_table);
	if (find == ANDEQ_M)
	{
		// halt: maybe deal with it later in encode
		return 0;
	}
	else if (find == LSL_M)
	{
		// special case
	}
	else if (find <= MOV_M)
	{
		// Data processing
		token_to_dpi(token, instruction, find, symbolTable);
		encode_DP(instruction->word.proc);
	}
	else if (find <= MLA_M)
	{
		// Multiply
	}
	else if (find <= STR_M)
	{
		// Single data transfer
	}
	else if (find <= B_M)
	{
		// Branch
	}

	free_symbol_table(mnemonic_table);

//	return instruction;
}

/* Know that the token is a dpi, add the component to instruction
 * */
void token_to_dpi(Token token, instruction_t* instruction, int opcode, SymbolTable symbol_table)
{
	allocate_address(token, symbol_table);
	instruction->tag = DATA_PROCESS;
	instruction->word.proc.cond = AL;
	instruction->word.proc.opcode = opcode;
//Their syntax is <opcode> Rd, Rn, <Operand2>
	switch (instruction->word.proc.opcode)
	{
	case ADD:
	case EOR:
	case SUB:
	case RSB:
	case AND:
	case ORR:
		instruction->word.proc.set_cond = 0;
		instruction->word.proc.Rd = token->operands[0].operand_data.number;
		instruction->word.proc.Rn = token->operands[1].operand_data.number;

		//op2 in the form of <#expression>
		int op2 = token->operands[2].operand_data.number;
		int* rotate_amount = (int*)malloc(sizeof(int));
		int* imm = (int*)malloc(sizeof(int));

		//If the numeric constant cannot be represented, give an error.
		if (!reverse_rotate(op2, rotate_amount, imm))
		{
			perror("the numeric constant cannot be represented.");
			exit(EXIT_FAILURE);
		}
		instruction->word.proc.operand2.rot_imm.imm = *imm;
		instruction->word.proc.operand2.rot_imm.amount = *rotate_amount;
		free(rotate_amount);
		free(imm);
		break;

		//Its syntax is: mov Rd, <Operand2>
	case MOV:
		instruction->word.proc.set_cond = 0;
		instruction->word.proc.Rd = token->operands[0].operand_data.number;

		int op2 = token->operands[1].operand_data.number;
		int* rotate_amount = (int*)malloc(sizeof(int));
		int* imm = (int*)malloc(sizeof(int));
		if (!reverse_rotate(op2, rotate_amount, imm))
		{
			perror("the numeric constant cannot be represented.");
			exit(EXIT_FAILURE);
		}
		instruction->word.proc.operand2.rot_imm.imm = *imm;
		instruction->word.proc.operand2.rot_imm.amount = *rotate_amount;
		free(rotate_amount);
		free(imm);

		break;

		//Their syntax is: <opcode> Rn, <Operand2>
	case TST:
	case TEQ:
	case CMP:
		instruction->word.proc.set_cond = 1;
		instruction->word.proc.Rn = token->operands[0].operand_data.number;

		int op2 = token->operands[1].operand_data.number;
		int* rotate_amount = (int*)malloc(sizeof(int));
		int* imm = (int*)malloc(sizeof(int));
		if (!reverse_rotate(op2, rotate_amount, imm))
		{
			perror("the numeric constant cannot be represented.");
			exit(EXIT_FAILURE);
		}
		instruction->word.proc.operand2.rot_imm.imm = *imm;
		instruction->word.proc.operand2.rot_imm.amount = *rotate_amount;
		free(rotate_amount);
		free(imm);

		break;
	}
}

void token_to_mul(Token token, instruction_t* instruction)
{
	instruction->tag = MULTIPLY;
	instruction->word.mul.cond = AL;
	operand_t operand = token->operands;
	instruction->word.mul.Rd = operand.operand_data.number;
	operand = operand.next;

	instruction->word.mul.Rm = operand.operand_data.number;
	operand = operand.next;

	instruction->word.mul.Rs = operand.operand_data.number;

	if (strcmp(token->opcode, "mul") == 0)
	{
		instruction->word.mul.Rn = 0;
		instruction->word.mul.acc = 0;
	}
	else if (strcmp(token->opcode, "mla") == 0)
	{
		operand = operand.next;
		instruction->word.mul.Rn = operand.operand_data.number;
		instruction->word.mul.acc = 1;
	}
}

void token_to_trans(Token token, instruction_t* instruction, SymbolTable symbolTable)
{
	allocate_address(token, symbolTable);
	instruction->tag = TRANS;
	instruction->word.trans.cond = AL;

	if (strcmp(token->opcode, "ldr") == 0)
	{
		instruction->word.trans.is_load = 1;
	}
	else
	{
		instruction->word.trans.is_load = 0;
	}

	operand_t op = token->operands;
	instruction->word.trans.Rd = op.operand_data.number;
	op = op.next; // op is the <address> now

	if (op.tag == NUMBER)
	{
		// <=expression> only possible for ldr instructions
		int expression = op.operand_data.number;
		if (expression < 0xFF)
		{
//				Token mov_token =
			instruction->tag = DATA_PROCESS;
			instruction->word.proc.opcode = MOV;
			// TODO: convert to a mov token
		}
		else
		{
			instruction->word.trans.is_up = 1;

			//todo: get current location and new one( op.dddd)
			// -8 considers the off-by-8 bytes effect of the pipeline
			instruction->word.trans.offset = data_address - find_symbol_table(token->opcode) - 8;

			instruction->word.trans.is_pre = 1;
		}
	}
	else
	{
		// Not dealing with optional
		instruction->word.trans.is_pre = 1;
		if (op.next == NULL)
		{
			// pre_indexed address specification
			/* At this point, op.tag = STRING, op.letters contains the following case
			 * case 1: [Rn] strlen = 4 ([R1]) or 5 ([R11], [R12])
			 * case 2: [Rn, <#expression>] strlen > 5([R1,#..])
			 * */
			instruction->word.trans.is_pre = 1;
			parse_preindexed_trans_operand(op, &instruction->word.trans);
		}
		else
		{
			// post-indexing
			instruction->word.trans.is_pre = 0;
			char* temp = op.operand_data.letters;
			instruction->word.trans.Rn = strtol(temp + 1, NULL, 0);
			op = op.next;

			if (op.operand_data.letters[0] == '#')
			{
				trans->offset = strtol(rest + 1, NULL, 0);
				trans->iFlag = 0;
				trans->is_up = 1;
			}
		}
	}
}

/*
 * case 1: [Rn]
 * case 2: [Rn,<#expression>]
 * case 3: [Rn,{+/-}Rm{,<shift>}]
 * */
// TODO: for later implementing optional, you can extract the part parsing
// 		<#expression> and {+/-}Rm{,<shift>}
void parse_preindexed_trans_operand(operand_t operand, trans_t* trans)
{
	char* letters = operand.operand_data.letters;

	if (strlen(letters) <= 5)
	{
		// [Rn]
		trans->Rn = operand.operand_data.number;
		trans->offset = 0;
		trans->iFlag = 0;
		trans->is_up = 1;
	}
	else
	{
		int length = strlen(letters) - 2;
		char temp[length]; // to store the string without []
		strncpy(temp, letters, length);

		// Parse it, Rn stores rn,
		// rest stores <#expression> or {+/-}Rm{,<shift>}
		char* Rn = strtok(temp, ",");
		char* rest = strtok(NULL, ",");
		trans.Rn = strtol(Rn + 1, NULL, 0);

		/*
		 * if <#expression>, #3 for example
		 * set offset
		 * the offset is an immediate value -> iFlag = 0
		 * set up bit
		 * */
		if (rest[0] == '#')
		{
			trans->offset = strtol(rest + 1, NULL, 0);
			trans->iFlag = 0;
			trans->is_up = 1;
		}
	}
}

uint32_t to_bcode_mov(Token cur_token)
{
	proc_t intermidiate_rep;

// retrive information from cur_token
// initialize intermidiate_rep
// call encode_DP

//intermidiate_rep.tag = DATA_PROCESS;
//intermidiate_rep.word.proc.cond = ?;
// intermidiate_rep.word.proc.iFlag ;
// intermidiate_rep.word.proc.opcode;
// intermidiate_rep.word.proc.operand2;
// intermidiate_rep.word.proc.Rd;
// intermidiate_rep.word.proc.Rn;
// intermidiate_rep.word.proc.set_cond;

	return encode_DP(intermidiate_rep);
}

void reg_imm_helper(bool is_imm, reg_or_imm_t reg_or_imm, uint32_t* target)
{
	if (is_imm)
	{
		set_bit_range(target, reg_or_imm.rot_imm.imm, 0, 7);
		set_bit_range(target, reg_or_imm.rot_imm.amount, 8, 11);
	}
	else
	{
		set_bit_range(target, reg_or_imm.shift_reg.Rm, 0, 3);
		set_bit_range(target, reg_or_imm.shift_reg.type, 5, 6);
		set_bit_range(target, reg_or_imm.shift_reg.val, 7, 11);
	}
}

uint32_t encode_DP(proc_t instruction)
{
	uint32_t result = 0;
	set_bit_range(&result, instruction.cond, 28, 31);
	set_bit(&result, instruction.iFlag, 25);
	set_bit_range(&result, instruction.opcode, 21, 24);
	set_bit(&result, instruction.set_cond, 20);
	set_bit_range(&result, instruction.Rn, 16, 19);
	set_bit_range(&result, instruction.Rd, 12, 15);
	reg_imm_helper(instruction.iFlag, instruction.operand2, result);
}

uint32_t encode_MUL(mul_t instruction)
{
	uint32_t result = 0;
	set_bit_range(&result, instruction.cond, 28, 31);
	set_bit(&result, instruction.acc, 21);
	set_bit(&result, instruction.set_cond, 20);
	set_bit_range(&result, instruction.Rd, 16, 19);
	set_bit_range(&result, instruction.Rn, 12, 15);
	set_bit_range(&result, instruction.Rs, 8, 11);
	set_bit_range(&result, instruction.Rm, 0, 3);
}

uint32_t encode_TRANS(trans_t instruction)
{
	uint32_t result = 0;
	set_bit_range(&result, instruction.cond, 28, 31);
	set_bit(&result, instruction.iFlag, 25);
	set_bit(&result, instruction.is_pre, 24);
	set_bit(&result, instruction.is_up, 23);
	set_bit(&result, instruction.is_load, 20);
	set_bit_range(&result, instruction.Rn, 16, 19);
	set_bit_range(&result, instruction.Rd, 12, 15);
	reg_imm_helper(!instruction.iFlag, instruction.offset, result);
}

uint32_t encode_BRANCH(branch_t instruction)
{
	uint32_t result = 0;
	set_bit_range(&result, instruction.cond, 28, 31);
	set_bit_range(&result, instruction.offset, 0, 23);
}

