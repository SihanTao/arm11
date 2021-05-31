#ifndef FILE_LOADER
#define FILE_LOADER

#define WORD_LENGTH 4


extern void read_file_to_mem(char const *file_name,
                             void *write_to, endian_mode mode);

#endif //FILE_LOADER
