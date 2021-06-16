#include "char_stream.h"

void next_char(CharStream char_stream)
{
  *char_stream += 1;
}

char get_char(CharStream char_stream)
{
  return **char_stream;
}

RecordPoint get_trace_back(CharStream char_stream)
{
  return *char_stream;
}

void do_trace_back(CharStream char_stream, RecordPoint record_point)
{
  *char_stream = record_point;
}