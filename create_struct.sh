#!/bin/bash

read -p "directory path (if you want in current directory leave blank): " loc_path

if [ -z "$loc_path" ]; then
    loc_path=$(pwd)
elif [ "$loc_path" = "null" ]; then
    loc_path=$(pwd)
fi

cd "$loc_path" || { echo "Error: Cannot change to directory $loc_path"; exit 1; }

read -p "project_name: " prog_name
mkdir -p "$prog_name"

git clone https://github.com/v01d-0w1/zsfml "$prog_name"

cd "$prog_name" || { echo "Error: Cannot enter project directory"; exit 1; }

export PROJ_NAME="$prog_name"

chmod +x feather.sh
./feather.sh
