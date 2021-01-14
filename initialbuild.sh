#!/bin/bash

BASEDIR=$(pwd)

mkdir build
cd build || exit
cmake -DCMAKE_INSTALL_PREFIX:PATH="$BASEDIR/install" ../.github/ 
cmake --build .
cd SampleServer-build || exit
ctest