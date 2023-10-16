#!/bin/sh


set -xeu

g++ app/main.cpp -g -o main `pkg-config --cflags --libs sdl2` -lSDL2_ttf 
./main


