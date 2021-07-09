# umati sample server

Sample-Server implementation of various umati endorsed OPC UA companion specifications. Provides a :whale: :package: to run locally for development purpose.

## Current Implementation

[![Gitpod ready-to-code](https://img.shields.io/badge/Gitpod-ready--to--code-blue?logo=gitpod)](https://gitpod.io/#https://github.com/umati/Sample-Server)

### Machine Tool Example

This is an example implementation of the OPC UA Machine Tool Companion Specification based on open62541. The server provides several simulated machine tool, which show different extension stages of the specification.

### Woodworking Example

This is an example implementation of the OPC UA Woodworking Companion Specification. The server provides two simulated woodworking machines, a basic one implementing only mandatory variables/objects and a full one implementing every optional variable/object. 

#### Scope

This implementation provides simulations for the machine tool and woodworking specification, which changes most values in a pseudo random way. As no real machine simulation is done in the background, the values need not correlate with each other. Also only reading of values of OPC UA-clients is implemented. All write requests are ignored silently.

#### Docker Image

This repository automatically builds a sample server docker image which is then run at `opc.tcp://opcua.umati.app:4840` (Security: none and higher / Authentication: anonymous)

Use this image for your development purposes. Pull this public docker image: `ghcr.io/umati/sample-server:main`

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

This implementation represents the OPC UA Instance with an instance of an C++ struct and connects them together. So writing an value in the C++ instance will also set this value in the corresponding OPC UA node. The binding is established by utilizing reflection (an improved version of the binding in [this publication](https://ieeexplore.ieee.org/document/8972189)).

The binding is done by comparing the name of structure members with the browse name of the OPC UA instance. As the binding uses memory addresses of the C++ instance, the instance itself must not be copied after the binding.

## Folder Structure

```text
├──cmake           CMake files for building the project, e.g. custom find-scripts
├──Exceptions      Custom exceptions
├──MachineTools    Simulated machine tools
├──model           Unpublished/Fixed NodeSet files
├──OpcUaTypes      C++ äquivalent of some OPC UA DataTypes and helpers for their definition
├──Robotics        Simulated Robots
├──tests           Some unit tests, initially created for tracking down bugs
├──TypeDefinition  Definition of C++ Types for OPC UA Types, that can be bind later
├──UmatiServerLib  Library for binding the defined types to OPC UA instances
```

## License

![GitHub](https://img.shields.io/github/license/umati/Sample-Server)

This sample server implementation is licensed under the [Mozilla Public License v2.0 (MPLv2)](LICENSE).
