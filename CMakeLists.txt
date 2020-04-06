cmake_minimum_required(VERSION 3.0)
project(ExampleMachinetoolServer)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(open62541 REQUIRED)
find_package(Open62541Cpp REQUIRED)
find_package(GTest REQUIRED NO_CMAKE_SYSTEM_PACKAGE_REGISTRY)

find_package(ReflCpp REQUIRED)


if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # needed or cmake doesn't recognize dependencies of generated files
  set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR})
endif ()

# Generate types and namespace for DI
ua_generate_nodeset_and_datatypes(
    NAME "di"
    FILE_CSV "${open62541_NODESET_DIR}/DI/OpcUaDiModel.csv"
    FILE_BSD "${open62541_NODESET_DIR}/DI/Opc.Ua.Di.Types.bsd"
    NAMESPACE_IDX 2
  FILE_NS "${open62541_NODESET_DIR}/DI/Opc.Ua.Di.NodeSet2.xml"
  OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
)
ua_generate_nodeid_header(
    NAME "machinery"
    FILE_CSV "${PROJECT_SOURCE_DIR}/model/machinery.csv"
    OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
    TARGET_SUFFIX "ids-machinery"
    ID_PREFIX "MACHINERY_"
)

ua_generate_nodeset(
    NAME "machinery"
    FILE "${PROJECT_SOURCE_DIR}/model/machinery.xml"
    OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
    # First NS0 Types, then other dependent namespaces
    DEPENDS_TYPES "UA_TYPES;UA_TYPES_DI"
    DEPENDS_NS "${open62541_NODESET_DIR}/Schema/Opc.Ua.NodeSet2.xml;${open62541_NODESET_DIR}/DI/Opc.Ua.Di.NodeSet2.xml"
)
# Workaround, as ua_generate_nodeset_and_datatypes expects a types-File.
file(TOUCH "${PROJECT_SOURCE_DIR}/src_generated/types_machinery_generated.h")

# ua_generate_nodeset_and_datatypes(
#     NAME "machinery"
#     FILE_NS "${PROJECT_SOURCE_DIR}/model/machinery.xml"
#     FILE_CSV "${PROJECT_SOURCE_DIR}/model/machinery.csv"
#     FILE_BSD "${PROJECT_SOURCE_DIR}/model/machinery.bsd"
#     OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
#     NAMESPACE_IDX 3
#     DEPENDS_TYPES "UA_TYPES_DI"
#     #DEPENDS_NS    "${UA_FILE_NS0}"
# )

ua_generate_nodeset_and_datatypes(
    NAME "machinetool"
    FILE_NS "${PROJECT_SOURCE_DIR}/model/machinetool.xml"
    FILE_CSV "${PROJECT_SOURCE_DIR}/model/machinetool.csv"
    FILE_BSD "${PROJECT_SOURCE_DIR}/model/machinetool.bsd"
    OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
    NAMESPACE_IDX 4
    DEPENDS_TYPES "UA_TYPES;UA_TYPES_DI;UA_TYPES_MACHINERY"
    DEPENDS di machinery
    #DEPENDS_NS    "${UA_FILE_NS0}"
)

ua_generate_nodeid_header(
    NAME "iswexample"
    FILE_CSV "${PROJECT_SOURCE_DIR}/model/iswexample.csv"
    OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
    TARGET_SUFFIX "ids-iswexample"
    ID_PREFIX "ISWEXAMPLE_"
)

ua_generate_nodeset(
    NAME "iswexample"
    FILE "${PROJECT_SOURCE_DIR}/model/iswexample.xml"
    OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
    # First NS0 Types, then other dependent namespaces
    DEPENDS_TYPES "UA_TYPES;UA_TYPES_DI;UA_TYPES_MACHINERY;UA_TYPES_MACHINETOOL"
    DEPENDS_NS "${open62541_NODESET_DIR}/Schema/Opc.Ua.NodeSet2.xml;${open62541_NODESET_DIR}/DI/Opc.Ua.Di.NodeSet2.xml;${PROJECT_SOURCE_DIR}/model/machinery.xml;${PROJECT_SOURCE_DIR}/model/machinetool.xml"
    #DEPENDS_NS "${open62541_NODESET_DIR}/Schema/Opc.Ua.NodeSet2.xml;${PROJECT_SOURCE_DIR}/model/machinery.xml;${PROJECT_SOURCE_DIR}/model/machinetool.xml;${open62541_NODESET_DIR}/DI/Opc.Ua.Di.NodeSet2.xml"
)

add_library(machinetool_generated
    # Add dependend files to trigger the generation!
    src_generated/namespace_di_generated.c
    src_generated/types_di_generated.h
    src_generated/di_nodeids.h
    src_generated/namespace_machinery_generated.c
    #src_generated/types_machinery_generated.h
    src_generated/machinery.h
    src_generated/namespace_machinetool_generated.c
    src_generated/types_machinetool_generated.h
    src_generated/machinetool_nodeids.h
    src_generated/namespace_iswexample_generated.c
    src_generated/iswexample.h
    )
target_link_libraries(machinetool_generated PUBLIC open62541::open62541)
set_target_properties(machinetool_generated PROPERTIES DEBUG_POSTFIX "d")

add_library(ExampleServerLib
    BindValue.cpp
    BindStruct.cpp
    BindRefl.cpp
    NodesMaster.cpp
    NodeValue.cpp
    Util.cpp
    OpcUaTypes/LocalizedText.cpp
    OpcUaTypes/EUInformation.cpp
    OpcUaTypes/ConstNodeId.cpp
)
set_target_properties(ExampleServerLib PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ExampleServerLib PROPERTIES DEBUG_POSTFIX "d")
target_link_libraries(ExampleServerLib PUBLIC open62541::open62541)
target_link_libraries(ExampleServerLib PUBLIC Open62541Cpp::Open62541Cpp)
target_link_libraries(ExampleServerLib PUBLIC reflCpp::reflCpp)

add_executable(
    ${PROJECT_NAME}
    SetupEvents.cpp
    ExampleMachinetoolServer.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
target_link_libraries(${PROJECT_NAME} PUBLIC ExampleServerLib)
target_link_libraries(${PROJECT_NAME} PUBLIC machinetool_generated)

add_subdirectory(tests)
include(CTest)
