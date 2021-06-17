#include "char_stream.h"

/*!
 *
 * @param char_stream
 * @return the next char in char_stream
 */
void next_char(CharStream char_stream)
{
  *char_stream += 1;
}

/*!
 *
 * @param char_stream
 * @return the char in char_stream
 */
char get_char(CharStream char_stream)
{
  return **char_stream;
}

/*!
 *
 * @param char_stream
 * @return char_stream in the type of RecordPoint
 */
RecordPoint get_trace_back(CharStream char_stream)
{
  return *char_stream;
}

/*!
 *
 * @param char_stream
 * @param record_point
 * @return : let char_stream = record_point
 */
void do_trace_back(CharStream char_stream, RecordPoint record_point)
{
  *char_stream = record_point;
}