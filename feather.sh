#!/bin/bash
g++ -c src/main.cpp -Iinclude
g++ src/main.o -o build/owl -Llib -lsfml-graphics -lsfml-window -lsfml-system
export LD_LIBRARY_PATH=lib && ./build/owl
