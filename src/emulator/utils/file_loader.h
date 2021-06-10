#ifndef FILE_LOADER
#define FILE_LOADER

#define WORD_LENGTH 4

/*
 * ## file_loader
 *
 * `file_loader` reads a binary file `file_name`, and write it to a array
 * `write_to`. It can read both big-endian binary file or small-endian binary
 * file.
 *
 * - `write_to` is assumed to be initialized with all 0s and has enough space
 *  to write to (at least 65536 bytes). It will write to `write_to` in
 *  small-endian mode by default.
 *
 * - `file_name`, the file is assumed have length of multiple of 4.
 *   Otherwise an error will be raised.
 */
extern void read_file_to_mem (char const *file_name, void *write_to,
                              endian_type mode);

#endif // FILE_LOADER
