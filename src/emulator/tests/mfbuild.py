#! /usr/bin/python3

import os
import subprocess
import re

all_test_c = [test_c for test_c in list(os.walk("."))[0][2]
              if ".c" in test_c]

include_dict = dict()

def handle_line(line):
    if "include" in line and '"' in line

for test_c in all_test_c:
    with open(test_c, "r") as test_c_handle:
        for line in test_c_handle:
            handle_line(line)


test_files = [subprocess.run(f"./{test}")
              for test in list(os.walk("."))[0][2]
              if ".test" in test]