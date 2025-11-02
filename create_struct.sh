#!/bin/bash

read -p "directory path(if you want in current directory leave blank):" loc_path

cd $loc_path

read -p "project_name:" prog_name
mkdir $prog_name
git clone https://github.com/v01d-0w1/zsfml $prog_name

export $prog_name
cd $prog_name
./feather.sh

