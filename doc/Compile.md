# Compile the Sample-Server

## Get the files

Clone the GitHub repository using your preferred approach.
This repository uses submodules, so be sure to clone them as well.

Standard option:
```bash
git clone --recurse-submodules https://github.com/umati/Sample-Server.git
```

## CMake for the Sample-Server

The submodules are dependencies for the Sample-Server and have to be built and installed in advance.
If possible on your system, execute the script `initialbuild.sh`, which calls cmake for all the submodules in the right order.

If this is not possible, execute the steps yourself.
Starting in the Sample-Servers topmost directory (the one that contains initialbuild.sh) create a build directory, e.g.
```bash
mkdir build
```

Change to that directory
```bash
cd build
```

Call cmake with your preferred/installed generator and the properties 
```CMake
CMAKE_BUILD_TYPE=Debug
CMAKE_INSTALL_PREFIX="[Sample-Server-Dir]/install" ../.github/
```
with \[Sample-Server-Dir\] being the Sample-Servers topmost directory.

Command-line for this cmake call:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX:PATH="[Sample-Server-Dir]/install" ../.github/
```

Generators that have worked in the past:
* gcc
* mingw
* Visual Studio 2017 or newer


If the cmake buildsystem was generated successfully, you can compile the project using said build system.
Example for command-line using cmake:
```bash
cmake --build .
```

## Run the server
The server executable can be found in \[Sample-Server-Dir\]/install/bin (the directory given to cmake with CMAKE_INSTALL_PREFIX).
Depending on the generator, there may also be an executable in the build directory (\[Sample-Server-Dir\]/build/SampleServer-build).
Given sufficient firewall permissions etc., this executable will start the server, listening on port 4840 for opc.tcp protocol.
