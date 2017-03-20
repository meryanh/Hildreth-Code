@echo off
g++ -Wall -std=gnu++0x -static -DGLFW_STATIC -DSOUNDIO_STATIC_LIBRARY -I. -Iinclude -Llib main.cpp -lsoundio -lglfw3 -lopengl32 -O3 -mwindows
strip a.exe