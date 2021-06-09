#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "helper.h"

uint32_t inverse_rotate(uint32_t target, int amount)
{
  int result = 0;
  for (int i = 31; i < 31-amount; i--)
  {
    if (get_bit(target, i) == 1)
    {
      result += pow(2, (31 - i));
    }
  }
  return result + (target << amount);
}