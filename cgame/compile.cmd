::     _ _ _     _           _       _      
::   _|_|_|_|   |_|         |_|_   _|_|   
::  |_|    _ _  |_|           |_|_|_|   
::  |_|   |_|_| |_|            _|_|_
::  |_|_ _ _|_| |_|_ _ _ _   _|_| |_|_
::    |_|_|_|   |_|_|_|_|_| |_|     |_|
@echo off
g++ -DFREEGLUT_STATIC main.cpp -o a.exe -std=gnu++0x -I"freeglut\include" -L"freeglut\lib" -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -lglu32 -Wl,--subsystem,windows -O3 -static
strip a.exe