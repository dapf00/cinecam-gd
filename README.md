# cinecam-gd Extension for CinePi and Godot

This is a Godot-Extension for CinePi, the Cinema Camera based on the Raspberry 5. Use this software in conjunction with this [fork](https://github.com/dapf00/cinepi-raw-fork) of CinePi-RAW and [CineCam](https://github.com/dapf00/cinecam).

# Build Instructions
Build the CinePi-RAW fork
Install dependecies
```bash
sudo apt install -y libcamera-dev libyuv-dev
```
Initialize Godot-cpp
```bash
git submodule init godot-cpp
git submodule update godot-cpp
```
Build Godot-cpp
```bash
cd godot-cpp
scons
```
Build debug version
```bash
cd ..
scons
```
Build release version
```bash
cd ..
scons target=release
```
Copy the binaries from results to the bin/ folder in CineCam-Godot

# Developer Info

To run static checks install cppcheck and clang-tidy and run
```
scons --no-build --clang-tidy --cppchecker
```
or 
```
scons --no-build --clang-tidy --cppchecker --check-file=src/example1.cpp --check-file=src/example2.hpp 
```
for only select files.
