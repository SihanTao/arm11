#include <stdint.h>
#include <stdbool.h>

#include "reverse_rotate.h"

// check whether 'target' is in the form of immediate value.

/*!
 * 
 * @param target
 * @return : determine whether the target is an immediate value or not
 */
static bool is_valid_imm(uint32_t target)
{
  uint32_t mask = 0X000000FF;
  if ((target & mask) == target)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/*!
 * 
 * @param target
 * @param rotation_amount
 * @param imm
 * @return the result after reverse rotate
 */
bool reverse_rotate(uint32_t target, int *rotation_amount, uint32_t *imm)
{
  if (is_valid_imm(target))
  {
    *rotation_amount = 0;
    *imm             = target;
    return true;
  }

  for (int i = 1; i < 16; i++)
  {
    target = (target << 2) | (target >> 30); // rotate left by 2.
    if (is_valid_imm(target))
    {
      *rotation_amount = i;
      *imm             = target;
      return true;
    }
  }

  return false;
}