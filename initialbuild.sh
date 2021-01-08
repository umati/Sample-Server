#bash 

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH="/workspace/Sample-Server/install" ../.github/ 
cmake --build .
cd SampleServer-build
ctest