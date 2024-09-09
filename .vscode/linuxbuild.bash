#!/bin/bash
workSpaceDir="$( realpath -- "$(dirname -- "$0")"/.. )"
cd "$workSpaceDir"
if [[ ! -a built ]]; then mkdir built; fi
cd built
g++ -O3 -std=c++17 -Wall -Wextra -Wpedantic -DNDEBUG $(sdl2-config --cflags) "$workSpaceDir"/*.cpp $(sdl2-config --libs) -o ./SDL2demo
