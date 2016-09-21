::     _ _ _     _           _       _      
::   _|_|_|_|   |_|         |_|_   _|_|   
::  |_|    _ _  |_|           |_|_|_|   
::  |_|   |_|_| |_|            _|_|_
::  |_|_ _ _|_| |_|_ _ _ _   _|_| |_|_
::    |_|_|_|   |_|_|_|_|_| |_|     |_|
@echo off
g++ -DFREEGLUT_STATIC test.cpp -o test.exe -std=c++0x -I"freeglut\include" -L"freeglut\lib" -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -lglu32 -Wl,--subsystem,windows -static