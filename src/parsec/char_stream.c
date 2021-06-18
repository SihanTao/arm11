#include "char_stream.h"

/*!
 *  the move the char stream pointer forward
 * @param char_stream
 */
void next_char(CharStream char_stream)
{
  *char_stream += 1;
}

/*!
 *
 * @param char_stream
 * @return the current char in char_stream
 */
char get_char(CharStream char_stream)
{
  return **char_stream;
}

/*!
 *
 * @param char_stream
 * @return generate a record point to trace back
 */
RecordPoint get_record_point(CharStream char_stream)
{
  return *char_stream;
}

/*!
 *
 * trace back to record point
 * @param char_stream
 * @param record_point
 */
void do_trace_back(CharStream char_stream, RecordPoint record_point)
{
  *char_stream = record_point;
}