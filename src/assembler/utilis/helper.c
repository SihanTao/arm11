#include "helper.h"

// check whether 'target' is in the form of immediate value.
static bool is_valid_imm(uint32_t target)
{
  uint32_t mask = 0X000000FF;
  if (target&mask == target) {
    return true;
  } else {
    return false;
  }
}

bool compute_imm(uint32_t target, int rotation_amount, uint32_t imm)
{
  if (is_valid_imm(target)) {
    rotation_amount = 0;
    imm = target;
    return true;
  }
   
  for (int i=1; i<16; i++) {
    target = (target << 2) | (target >>30); // rotate left by 2.
    if (is_valid_imm(target)) {
      rotation_amount = i;
      imm = target;
      return true;
    }
  }

   return false;
}