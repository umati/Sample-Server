# umati sample server

Sample-Server implementation of various umati endorsed OPC UA companion specifications. Provides a :whale: :package: to run locally for development purpose.

## Current Implementation

[![Gitpod ready-to-code](https://img.shields.io/badge/Gitpod-ready--to--code-blue?logo=gitpod)](https://gitpod.io/#https://github.com/umati/Sample-Server)

### Machine Tool Examples

This is an example implementation of the OPC UA Machine Tool Companion Specification based on open62541. The server provides several simulated machine tools, which show different extension stages of the specification.

### Woodworking Examples

This is an example implementation of the OPC UA Woodworking Companion Specification. The server provides two simulated woodworking machines, a basic one implementing only mandatory variables/objects and a full one implementing every optional variable/object.

#### Scope

This implementation provides simulationed instances for the machine tool and woodworking specification, which changes most values in a pseudo random way. As no real machine simulation is done in the background, the values need not correlate with each other. Also only reading of values of OPC UA-clients is implemented. All write requests are ignored silently.

#### Docker Image

This repository automatically builds a sample server docker image which is then run at `opc.tcp://opcua.umati.app:4840` (Security: none and higher / Authentication: anonymous)

Use this container image for your local development purposes. Pull this public image at: `docker pull ghcr.io/umati/sample-server:main`

To run this image you need a docker installation and than typically:
`docker run -d -p 4840:4840 ghcr.io/umati/sample-server:main`

Refer to the [docker documentation](https://docs.docker.com/) for details.

### Build status

![GitHub Workflow Status](https://img.shields.io/github/workflow/status/umati/Sample-Server/Build)

### Configuration

The configuration uses a json format. By default, the server looks for a file `configuration.json`. The supported parameters are:

```json
{
    "Hostname": "localhost", // Hostname of the server, should be identical to the hostname that is used by OPC UA clients to connect to the server.
    "Encryption": { // Optional encryption, can be omitted to only support unencrypted connections
        "ServerCert": "server_cert.der", // Server certificate
        "ServerKey": "server_key.der", // Server private key for the certificate
        "TrustedClients": ["trusted/test.pem"], // List of allowed clients, if empty, all client certificates are accepted
        "IssuerCerts": [], // List of allowed issuers
        "Revocation": [] // List of revoked certificates.
    }
}
```

## Concept

This implementation represents the OPC UA Instance with an instance of an C++ struct and connects them together. So writing an value in the C++ instance will also set this value in the corresponding `OPC UA node`. The binding is established by utilizing reflection (an improved version of the binding in [this publication](https://ieeexplore.ieee.org/document/8972189)).

The binding is done by comparing the name of structure members with the browse name of the OPC UA instance. As the binding uses memory addresses of the C++ instance, the instance itself must not be copied after the binding.

## Folder Structure

```text
├──arch            architecture specific files
├──cmake           CMake files for building the project, e.g. custom find-scripts
├──Exceptions      Custom exceptions
├──MachineTools    Simulated machine tools
├──model           Unpublished/Fixed NodeSet files
├──OpcUaTypes      C++ equivalent of some OPC UA DataTypes and helpers for their definition
├──Robotics        Simulated Robots
├──tests           Some unit tests, initially created for tracking down bugs
├──tools/certGen   Custom certificate generator tool           
├──TypeDefinition  Definition of C++ Types for OPC UA Types, that can be bind later
├──UmatiServerLib  Library for binding the defined types to OPC UA instances
├──Woodworkng      Simulated wood working sample machines
```
## Example Build under Windows
Dependencies:
- Install and configure MinGW as described here https://code.visualstudio.com/docs/cpp/config-mingw
- Install CMAKE https://cmake.org/download/

```powershell
mkdir _build
mkdir _install
mkdir _deps
cd _deps
cmake ..\.github\ -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX:PATH="/Users/ac128531/Desktop/Sample-Server/Sample-Server/_install" -DUA_ENABLE_MQTT:BOOL=1
```
Edit `_deps\open62541External-prefix\src\open62541External-build\cmake_install.cmake` (See EXAMPLE diff below!).
```diff
diff --git a/./a.cmake b/./b.cmake
index fa4a524..2949891 100644
--- a/./a.cmake
+++ b/./b.cmake
@@ -238,11 +238,11 @@ if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_
 endif()

 if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
-  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/open62541" TYPE FILE FILES "C:/Users/ac128531/Desktop/Sample-Server/Sample-Server/deps/open62541/plugins/ua_network_pubsub_mqtt.h")
+  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/open62541C:/Users/ac128531/Desktop/Sample-Server/Sample-Server/deps/open62541/plugins" TYPE FILE FILES "C:/Users/ac128531/Desktop/Sample-Server/Sample-Server/deps/open62541/plugins/ua_network_pubsub_mqtt.h")
 endif()

 if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
-  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/open62541" TYPE FILE FILES "C:/Users/ac128531/Desktop/Sample-Server/Sample-Server/deps/open62541/plugins/mqtt/ua_mqtt_adapter.h")
+  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/open62541C:/Users/ac128531/Desktop/Sample-Server/Sample-Server/deps/open62541/plugins/mqtt" TYPE FILE FILES "C:/Users/ac128531/Desktop/Sample-Server/Sample-Server/deps/open62541/plugins/mqtt/ua_mqtt_adapter.h")
 endif()

 if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
```
```powershell
cmake --build .
```
The exe can be found under `<sourceDirectory>/_install/bin`. Put the `configuration.json` (see `configuration.template.json`) in this
directory.

`.vscode/settings.json` for debugging with C/C++ and cmake extensions for vscode:
```json
{
    "cmake.sourceDirectory": "${workspaceFolder}",
    "cmake.buildDirectory": "${workspaceFolder}/_build",
    "cmake.installPrefix": "${workspaceFolder}/_install",
    "cmake.generator": "MinGW Makefiles"
}
```



## License

![GitHub](https://img.shields.io/github/license/umati/Sample-Server)

This sample server implementation is licensed under the [Mozilla Public License v2.0 (MPLv2)](LICENSE) except otherwise stated in the header of a file.
