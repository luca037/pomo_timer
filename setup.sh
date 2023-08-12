#!/bin/bash

# create file calendar.txt
if [ ! -e "calendar.txt" ]; then
    touch "calendar.txt"
fi

# create build dir
if [ ! -d "build" ]; then
    mkdir "build"
fi

# compile
cd ./build
cmake ..
make
cd ..
