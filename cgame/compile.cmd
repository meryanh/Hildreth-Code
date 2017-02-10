@echo off
g++ main.cpp -o a.exe -std=gnu++0x -static -DGLFW_STATIC -I. -Iinclude/ -Llib/ -lopenal32 -lglfw3 -lopengl32 -lglu32 -lgdi32 -lm -O2 -mwindows
strip a.exe