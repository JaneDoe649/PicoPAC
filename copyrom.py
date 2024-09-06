import os
import shutil

def copy_files(src_dir, dst_dir):

    for root, dirs, files in os.walk(src_dir):
        for file in files:
            first_letter = file[0].upper()

            dst_subdir = os.path.join(dst_dir, first_letter)
            if not os.path.exists(dst_subdir):
                os.makedirs(dst_subdir)

            shutil.copy(os.path.join(root, file), dst_subdir)

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 3:
        print("Usage: python script.py <src_dir> <dst_dir>")
        sys.exit(1)

    src_dir = sys.argv[1]
    dst_dir = sys.argv[2]
    copy_files(src_dir, dst_dir)
    