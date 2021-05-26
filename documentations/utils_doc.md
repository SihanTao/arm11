## To-do

[ ] file_loader big-endian small-endian support

## file_loader

`file_loader` reads a binary file `file_name`, and write it to a array `write_to`. It can read both big-endian binary file or small-endian binary file.

- `write_to` is assumed to be initialized with all 0s and has enough space to write to (at least 65536 bytes). It will write to `write_to` in big-endian mode.

- `file_name`, the file is assumed have length of multiple of 4. Otherwise an error will be raised.

## unit_test

usage

Before each test, call the function `add_test`.

`test_true` and `test_false` can test true and false. They can report which test failed.

`test_eq` can tell whether two objects have the same memory content or not. It can also report which test failed.

`add_test` also summarize the test information. i.e. report how many test failed or passed.

At the end of the test, you should call `end_test` explicitly, such that it can summarize the last test.