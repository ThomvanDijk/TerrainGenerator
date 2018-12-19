#!/bin/bash

# delete build directory
if [ -d build ]; then
	rm -rf build
fi

# make new build directory
mkdir -p build

# go to that directory
cd build

# build the project
cmake ..
