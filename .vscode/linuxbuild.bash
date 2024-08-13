#!/bin/bash
if [[ ! -a built ]]; then mkdir built; fi
cd built
g++ -O3 -std=c++17 -Wall -Wextra -Wpedantic -DNDEBUG $(realpath ..)/*.cpp -lSDL2 -o ./SDL2demo
