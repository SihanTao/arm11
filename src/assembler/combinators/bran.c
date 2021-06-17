#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"

#include "../../parsec/parsec.h"

#include "../../global_utils/types_and_macros.h"

#include "../data_structure/symbol_table.h"

#include "bran.h"

/*!
 * @return a parser combinator condition part of a branch instruction.
*/

Parsec p_bran_cond(void)
{
  Parsec alts[8]
      = { match("beq", "beq "), match("bne", "bne "), match("bge", "bge "),
          match("blt", "blt "), match("bgt", "bgt "), match("ble", "ble "),
          match("bal", "bal "), match("bal", "b ") };
  return alt("bran_cond", alts, 8);
}

/*!
 * @param bran_cond an AST type which is the condition of a branch instructio.
 * @return the result of encoding.
*/
cond_type e_bran_cond(AST bran_cond)
{
  AST cond = $G(bran_cond, "beq");
  if (cond)
  {
    return EQ;
  }

  cond = $G(bran_cond, "bne");
  if (cond)
  {
    return NE;
  }

  cond = $G(bran_cond, "bge");
  if (cond)
  {
    return GE;
  }

  cond = $G(bran_cond, "blt");
  if (cond)
  {
    return LT;
  }

  cond = $G(bran_cond, "bgt");
  if (cond)
  {
    return GT;
  }

  cond = $G(bran_cond, "ble");
  if (cond)
  {
    return LE;
  }

  cond = $G(bran_cond, "bal");
  if (cond)
  {
    return AL;
  }
}

/*!
 * @param bran
 * @param symbol_table
 * @param current_address
 * @return the encoded branch insturction.
*/
instruction_t e_bran(AST bran, SymbolTable symbol_table, int current_address)
{
  instruction_t result;
  result.cond = e_bran_cond($G(bran, "bran_cond"));
  result.tag  = BRAN;
  printf("current_address :>> %d\n", current_address); //DELETE_MARK
  result.word.branch.offset
      = find_symbol_table($TG(bran, "label"), symbol_table) - current_address - 8;

  printf("result.word.branch.offset :>> %d\n", result.word.branch.offset); //DELETE_MARK
  return result;
}

Parsec p_bran(void)
{
  Parsec seqs[3] = {p_bran_cond(), take_while("label", isalpha), match(NULL, "\n")};
  return seq("bran", seqs, 3);
}