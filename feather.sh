#!/bin/bash

game_name="owl"
if [ -n "$prog_name" ] && [ "$prog_name" != "null" ]; then
    game_name="$prog_name"
fi

echo "Building game: $game_name"

g++ -c src/main.cpp -Iinclude -o src/main.o
g++ src/main.o -o build/$game_name -Llib -lsfml-graphics -lsfml-window -lsfml-system
export LD_LIBRARY_PATH=lib && ./build/$game_name

