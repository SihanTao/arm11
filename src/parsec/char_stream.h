#ifndef CHAR_STREAM
#define CHAR_STREAM

typedef char * RecordPoint;
typedef char **CharStream;

void next_char(CharStream char_stream);
char get_char(CharStream char_stream);
RecordPoint get_record_point(CharStream char_stream);
void do_trace_back(CharStream char_stream,
                   RecordPoint record_point);

#endif //CHAR_STREAM



