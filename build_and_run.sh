#!/bin/bash

set -e 

rm -r build
echo "Clean up finished"
echo ""

mkdir build
cd build

echo "Start building..."
echo ""
cmake ..
echo "Building finished"
echo ""

echo "Start compiling..."
echo ""
make 
echo "Compiling finished"
echo ""

echo "Starting the program..."
echo ""
./Program
echo ""
echo "Closing the program"
