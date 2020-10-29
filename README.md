# Machine Tool Example
This is an example implementation of the OPC UA Machine Tool Companion Specification based on open62541. The server provides several simulated machine tool, which show different extension stages of the specification.

# Scope
This implementation provides simulations for the machine tool specification, which changes most values in a pseudo random way. As no real machine simulation is done in the background, the values need not correlate with each other. Also only reading of values of OPC UA-clients is implemented. All write requests are ignored silently. 

# Concept
This implementation represents the OPC UA Instance with an instance of an C++ struct and connects them together. So writing an value in the C++ instance will also set this value in the corresponding OPC UA node. The binding is established by utilizing reflection (an improved version of the binding in [this publication](https://ieeexplore.ieee.org/document/8972189)).
The binding is done by comparing the name of structure members with the browse name of the OPC UA instance. As the binding uses memory addresses of the C++ instance, the instance itself must not be copied after the binding.

# Folder Structure
```
├──cmake     CMake files for building the project, e.g. custom find-scripts
├──Exceptions      Custom exceptions
├──MachineTools      Simulated machine tools
├──model     Unpublished/Fixed NodeSet files
├──OpcUaTypes      C++ äquivalent of some OPC UA DataTypes and helpers for their definition
├──Robotics      Simulated Robots
├──tests     Some unit tests, initially created for tracking down bugs
├──TypeDefinition      Definition of C++ Types for OPC UA Types, that can be bind later
├──UmatiServerLib     Library for binding the defined types to OPC UA instances
```

