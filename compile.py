import os
import subprocess
import argparse
import shutil
from pathlib import Path
from collections import defaultdict


# first compile asm files
# next, c++ files
# find all assembly sections

def compileAsmLinux(src_path, out_path):
    commands = [
        'nasm', '-f elf64',
        src_path,
        '-o', out_path
    ]
    try:
        subprocess.run(commands, check=True)
        print(f"[+] Assembled: {src_path} -> {out_path}")
    except subprocess.CalledProcessError as e:
        print(f"[!] Failed to assemble {src_path}: {e}")
        print(f"    Command: {' '.join(commands)}")


def compileCppLinux(src_path, out_path):
    commands = [
        'clang++', 
        '-ffreestanding','-nostdlib','-mno-red-zone','-fno-exceptions','-fno-rtti','-fno-builtin',
        '-c',
        src_path,
        '-o', out_path
    ]
    try:
        subprocess.run(commands, check=True)
        print(f"[+] Compiled: {src_path} -> {out_path}")
    except subprocess.CalledProcessError as e:
        print(f"[!] Failed to compile {src_path}: {e}")
        print(f"    Command: {' '.join(commands)}")

def linkLinux(linkScript, objFiles, output):
    commands = [
        'ld', '-n',
        '-T', linkScript,
        *(objFiles),
        '-o', output
    ]
    try:
        subprocess.run(commands, check=True)
        print(f"[+] Linked: {Path(objFiles[0]).parent} -> {output}")
    except subprocess.CalledProcessError as e:
        print(f"[!] Failed to link {Path(objFiles[0]).parent}: {e}")
        print(f"    Command: {' '.join(commands)}")

obj_files = defaultdict(list)

BUILD_DIR = Path("build")
BIN_DIR   = Path("bin")
ISO_DIR   = Path("iso")
GRUB_DIR  = Path("bin/boot/grub")
SRC_DIR   = Path("src")

if not BIN_DIR.exists():
    BIN_DIR.mkdir(parents=True, exist_ok=True)

if not ISO_DIR.exists():
    ISO_DIR.mkdir(parents=True, exist_ok=True)

if not GRUB_DIR.exists():
    GRUB_DIR.mkdir(parents=True, exist_ok=True)

if os.name == 'posix':
    #Walk directory and process .asm files
    for srcdir in SRC_DIR.iterdir():
        # foreach dir, create build/dir
        # create object files
        # link files with output to bin/dir.bin
        if not srcdir.is_dir():
            continue
        
        print(f"Processing directory: {srcdir}")
        buildSubdir = BUILD_DIR / srcdir.name
        binFile     = BIN_DIR / f"{srcdir.name}.bin"
        objFiles    = []
                
        if not buildSubdir.exists():
            buildSubdir.mkdir(parents=True, exist_ok=True)
        
        for file in srcdir.rglob('*'):
            if file.suffix == ".cpp": 
                obj = buildSubdir / file.with_suffix(".o").name
                compileCppLinux(str(file), str(obj))
                objFiles.append(str(obj))
            if file.suffix == ".asm": 
                obj = buildSubdir / file.with_suffix(".o").name
                compileAsmLinux(str(file) , str(obj))
                objFiles.append(str(obj))

        if objFiles and Path(srcdir / 'linker.ld').exists():
            linkLinux(str(srcdir / 'linker.ld'), objFiles, binFile)
        else:
            print(f"[!] No object files found in {srcdir}, skipping linking.")

    shutil.copyfile('grub.cfg', GRUB_DIR / 'grub.cfg')

    try:
        subprocess.run(['sh', './check.sh'], check=True)

        subprocess.run(['grub-mkrescue', '-o', 'iso/ambarOS.iso', BIN_DIR], check=True)
        print("[+] Created ambarOS.iso")
    except subprocess.CalledProcessError as e:
        print(f"[!] Failed to create ISO: {e}")
