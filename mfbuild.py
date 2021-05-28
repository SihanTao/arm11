import os
import subprocess

sub_dirs = list(os.walk("./src"))

file_dir_map = dict()
file_depen_map = dict()

# record_dir

for folder_name, _, sub_files in sub_dirs:
    for file_name in sub_files:
        if ".c" in file_name:
            file_dir_map.__setitem__(file_name, folder_name)

all_files = [j for j in
             sum([i[2] for i in sub_dirs], [])
             if ".c" in j]

all_tests = [j for j in
             [i[2] for i in sub_dirs if i[0] == "./src/tests"][0]
             if ".c" in j]

def generate_dependency_tree():
    for file_name, file_folder in all_files:
        full_name = os.path.join(file_folder, file_name)
        with open(full_name, "w") as file_c:
            for line in file_c:
                if "include" in line and '"' in line:
                    add_depen(file_name, line)
                elif "{" in line:
                    break


print("haven't finished yet!")

# for test_c in all_test_c:
#     with open(test_c, "r") as test_c_handle:
#         for line in test_c_handle:
#             handle_line(line)
