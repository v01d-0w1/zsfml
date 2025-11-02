#!/bin/bash

game_name="owl"
if [ -n "$PROJ_NAME" ] && [ "$PROJ_NAME" != "null" ]; then
    game_name="$PROJ_NAME"
fi

echo "Building game: $game_name"

g++ -c src/main.cpp -Iinclude -o src/main.o
g++ src/main.o -o build/$game_name -Llib -lsfml-graphics -lsfml-window -lsfml-system
export LD_LIBRARY_PATH=lib && ./build/$game_name

