#!/bin/bash
# Simple build script
g++ main.cpp -o main.exe \
-I/mingw64/include -L/mingw64/lib \
-lglfw3 -lglew32 -lopengl32 -lgdi32 \
-Wl,--subsystem,windows

# Run immediately after building
./main.exe