#include <stdlib.h>

#include "../../parsec/ast.h"

#include "token_stream.h"

/*!
 * add the given element to the end of token stream
 * @param elem
 * @param stream
 */
void add_token_stream(token_t* elem, TokenStream stream)
{
  if (stream->head == NULL)
  {
    stream->head = elem;
    stream->tail = elem;
    return;
  }

  stream->tail->next = elem;
  stream->tail = elem;
}

TokenStream init_token_stream()
{
  return malloc(sizeof(token_stream_t));
}

/*!
 * free a token stream
 * @param stream
 */
void free_token_stream(TokenStream stream)
{
  free_token(stream->head);
  free(stream);
}
/*!
 * free one token
 * @param token
 */
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
