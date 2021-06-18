#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"

#include "../../parsec/parsec.h"

#include "../../global_utils/types_and_macros.h"

#include "../data_structure/symbol_table.h"

#include "bran.h"

/*!
 * branch condition can be (beq | bne | bge | blt | bgt | ble | bal | b)
 * @return
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
 * @param bran_cond an AST type which is the condition of a branch instruction
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
 * convert branch AST to instruction_t
 * @param bran
 * @param symbol_table to retrieve the stored address of label
 * @param current_address to calculate offset
 * @return the encoded branch insturction.
 */
instruction_t e_bran(AST bran, SymbolTable symbol_table, int current_address)
{
  instruction_t result;

  result.word.branch.offset
      = (find_symbol_table($TG(bran, "label"), symbol_table) - current_address
         - 2 * ADDRESS_SHIFT)
        >> 2;
  result.cond = e_bran_cond($G(bran, "bran_cond"));
  result.tag  = BRAN;
  return result;
}

/*!
 * a branch instruction is a (branch condition + a label)
 * @return the encoded branch insturction.
 */
Parsec p_bran(void)
{
  Parsec seqs[3]
      = { p_bran_cond(), take_while("label", isalnum), match(NULL, "\n") };
  return seq("bran", seqs, 3);
}