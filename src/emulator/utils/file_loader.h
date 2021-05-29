#ifndef FILE_LOADER
#define FILE_LOADER

#define WORD_LENGTH 4

typedef enum endian_mode
{
  big,
  little
} endian_mode;

extern void read_file_to_mem(char const *file_name,
                             char *write_to, endian_mode mode);

#endif //FILE_LOADER
