#!/usr/bin/env python
import os
import sys
import subprocess
from pathlib import Path

AddOption('--no-build', dest='no-build', action='store_true', default=False, help='Don\'t build project.') 

AddOption('--clang-tidy', dest='clang-tidy', action='store_true', default=False, help='Checks project/files with clang-tidy.') 

AddOption('--cppchecker', dest='cppchecker', action='store_true', default=False, help='Checks project/files with cppchecker.') 

AddOption('--check-file', dest='check-files', action='append', help='Only check these files. Example: --check-file=src/example1.cpp --check-file=src/example2.cpp')

def run_clang_tidy(env, source): 
    path = os.path.dirname(os.path.abspath('__file__'))
    if GetOption('check-files')!=None:
        for src in GetOption('check-files'): 
            cmd = f"clang-tidy {path}/{src} -- -std=c++17 -I /usr/include/libcamera -I {path}/godot-cpp/include -I {path}/godot-cpp/gdextension -I {path}/godot-cpp/gen/include" 
            subprocess.run(cmd, shell=True, check=True)
    else:
        for src in source: 
            cmd = f"clang-tidy {src} -- -std=c++17 -I /usr/include/libcamera -I {path}/godot-cpp/include -I {path}/godot-cpp/gdextension -I {path}/godot-cpp/gen/include" 
            subprocess.run(cmd, shell=True, check=True)

def run_cppcheck(env): 
    sources = "src"
    if GetOption('check-files')!=None:
       sources = " ".join(GetOption('check-files')) 
    print(sources)
    path = os.path.dirname(os.path.abspath('__file__'))
    cmd = f"cppcheck --enable=all --inline-suppr --disable=missingInclude --inconclusive --std=c++17 --language=c++ --template=gcc {sources}" 
    subprocess.run(cmd, shell=True, check=False)

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags
# tweak this if you want to use different folders, or more folders, to store your source code in.

env = SConscript("godot-cpp/SConstruct")
repo_root = os.path.dirname(os.path.abspath('__file__'))
env.Append(CPPPATH=[
    '/usr/include/libcamera',
    '/usr/local/include/libcamera',
    '/usr/local/include/libyuv/' 
])

# Library paths
env.Append(LIBPATH=[
    '/usr/lib/aarch64-linux-gnu',
    '/usr/lib/lib' 
])

# Libraries to link
env.Append(LIBS=['camera', 'yuv'])

# Optional: Set RPATH
env.Append(LINKFLAGS=['-Wl,-rpath,/usr/lib/lib'])

env.Append(CCFLAGS=['-march=armv9-a+sve2+simd'])
env.Append(CPPPATH=["src/"])

sources = Glob("src/*.cpp")
clang_tidy_sources = Glob("src/*.[ch]pp")

if GetOption('clang-tidy'):
    run_clang_tidy(env, clang_tidy_sources)
if GetOption('cppchecker'):
    run_cppcheck(env)


if GetOption('no-build') == False:
    if env["platform"] == "macos":
        library = env.SharedLibrary(
            "result/bin/cinepi-gd.{}.{}.framework/libcinepi-gd.{}.{}".format(
                env["platform"], env["target"], env["platform"], env["target"]
            ),
            source=sources,
        )
    else:
        library = env.SharedLibrary(
            "result/bin/cinepi-gd{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
            source=sources,
        )
    Default(library)
