#include <stdlib.h>

#include "token_stream.h"

void add_token_stream(token_t* elem, TokenStream stream)
{
  if (stream->head == NULL)
  {
    stream->head = elem;
    stream->tail = elem;
  }

  stream->tail->next = elem;
}

TokenStream init_token_stream()
{
  return malloc(sizeof(token_stream_t));
}

void free_token_stream(TokenStream stream)
{
  free_token(stream->head);
}

void free_token(Token token)
{
  if (token == NULL)
  {
    return;
  }

  for (int i = 0; i < token->num_operand; i++)
  {
    free(token->operands + i);
  }
  
  free_token(token->next);
  free(token);
}
