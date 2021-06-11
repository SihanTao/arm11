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
  //not sure how strtok work, leave for future
}


