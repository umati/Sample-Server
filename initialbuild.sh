#bash

BASEDIR=$(pwd)

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH="$BASEDIR/install" ../.github/ 
cmake --build .
cd SampleServer-build
ctest