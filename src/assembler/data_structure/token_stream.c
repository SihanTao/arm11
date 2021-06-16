#include <stdlib.h>

#include "../../parsec/ast.h"

#include "token_stream.h"

/*!
 *
 * @param elem
 * @param stream
 * @return : add the given element to the token stream
 */
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

/*!
 *
 * @param stream
 * @return the head of the token stream
 */
Token pop_token_stream(TokenStream stream)
{
  Token result = stream->head;
  if (result == NULL)
  {
    return NULL;
  }
  stream->head = result->next;
  return result;
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

  free_ast(token->ast);

  free_token(token->next);
  free(token);
}
