#include <stdbool.h>

#include "token_stream.h"

bool is_end(token_t current)
{
  return current.opcode == 0;
}