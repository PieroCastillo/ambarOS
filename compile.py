import os
import subprocess
import argparse
import shutil
from collections import defaultdict


# first compile asm files
# next, c++ files
# find all assembly sections

def compileAsmNT(src_path, out_path):
    try:
        subprocess.run([
            'powershell', '-command', 'as',
            src_path,
            '-o', out_path
        ], check=True)
        print(f"[+] Assembled: {src_path} -> {out_path}")
    except subprocess.CalledProcessError as e:
        print(f"[!] Failed to assemble {src_path}: {e}")


def compileCppNT(src_path, out_path):
    try:
        subprocess.run([
            'powershell', '-command', 'gcc', ' -ffreestanding',' -nostdlib',' -mno-red-zone',' -fno-exceptions',' -fno-rtti',' -fno-builtin',' -m64 ', src_path,
            ' -o ', out_path
        ], check=True)
        print(f"[+] Compiled: {src_path} -> {out_path}")
    except subprocess.CalledProcessError as e:
        print(f"[!] Failed to compile {src_path}: {e}")


# def list_sections(obj_path):
#     """
#     List sections of an object/binary using readelf.
#     """
#     try:
#         result = subprocess.run(
#             ['readelf', '-S', obj_path],
#             check=True,
#             stdout=subprocess.PIPE,
#             stderr=subprocess.PIPE,
#             text=True
#         )
#         print(f"\n=== Sections for {obj_path} ===")
#         print(result.stdout)
#     except subprocess.CalledProcessError as e:
#         print(f"[!] Failed to list sections for {obj_path}: {e.stderr}")

obj_files = defaultdict(list)
BuildDir = "build\\"
BinDir = "bin\\"
grubDir = "bin\\boot\\grub\\"
SourceDirs = ["volta"]

if not os.path.exists(BuildDir):
    os.makedirs(BuildDir)

if not os.path.exists(BinDir):
    os.makedirs(BinDir)

if not os.path.exists(grubDir):
    os.makedirs(grubDir)

if os.name == 'nt':
    #Walk directory and process .asm files
    for root_dir in SourceDirs:
        for dirpath, _, filenames in os.walk(root_dir):
            if not os.path.exists(BuildDir + dirpath):
                os.makedirs(BuildDir + dirpath)
            for fname in filenames:
                if fname.lower().endswith('.asm'):
                    src = os.path.join(dirpath, fname)
                    obj = BuildDir + os.path.splitext(src)[0] + '.o'
                    compileAsmNT(src, obj)
                    obj_files[root_dir].append(obj)

    # Walk directory and process .cpp files
    for root_dir in SourceDirs:
        for dirpath, _, filenames in os.walk(root_dir):
            if not os.path.exists(BuildDir + dirpath):
                os.makedirs(BuildDir + dirpath)
            for fname in filenames:
                if fname.lower().endswith('.cpp'):
                    src = os.path.join(dirpath, fname)
                    obj = BuildDir + os.path.splitext(src)[0] + '.o'
                    compileCppNT(src, obj)
                    obj_files[root_dir].append(obj)

    for dir in SourceDirs:
        linkFile = 'link.ld'
        linkPath = os.path.join(dir, linkFile)
        localBuildDir = BuildDir + dir
        if os.path.exists(linkPath):
            try:
                subprocess.run([
                    'powershell', '-command', 'gcc', '-T', linkPath,
                    '-o', BinDir + '\\' + dir + '.bin', '-ffreestanding', '-O2', '-nostdlib',
                    *(obj_files[dir]), '-lgcc'
                ], check=True)
                print(f"[+] Linked: {localBuildDir} -> {BinDir}.bin")
            except subprocess.CalledProcessError as e:
                print(f"[!] Failed to link {localBuildDir}: {e}")
    shutil.copyfile('grub.cfg', grubDir + '//grub.cfg')
    # # List sections of each object file
    # for obj in obj_files:
    #     if os.path.exists(obj):
    #         list_sections(obj)
    #     else:
    #         print(f"[!] Object file not found: {obj}")