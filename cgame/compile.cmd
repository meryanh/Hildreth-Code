@echo off
g++ -Wall -std=gnu++0x -static -DGLFW_STATIC -DSOUNDIO_STATIC_LIBRARY -I. -Iinclude -Llib main.cpp -lsoundio -lole32 -lglfw3 -lopengl32 -lglu32 -lgdi32 -lm -O2 -mwindows
strip a.exe