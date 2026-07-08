#!/bin/bash

mkdir -p ../build
cd ../build
if [ -f "data.txt" ]; then
    rm data.txt
fi
touch ../build/data.txt
cd ../app
make all
cd ../src
cd ../build
./main.exe
