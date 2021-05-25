#include "bit_opearion.h"
#include <stdio.h>
#include <stdint.h>

#define INT_BIT_LENGTH 32

void print_bit(uint32_t i) {
  for (int k = INT_BIT_LENGTH - 1; k >= 0; k--) {
    printf("%d", get_k_bit(i, k));
  }
  printf("\n");
}

int get_k_bit(uint32_t i, int k) {
  return (i & (1 << k)) >> k;
}
