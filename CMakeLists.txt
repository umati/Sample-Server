cmake_minimum_required(VERSION 3.15)
project(ExampleMachinetoolServer VERSION 0.1)

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

# See https://github.com/open62541/open62541/issues/3747
ua_generate_nodeid_header(
    NAME "machinery"
    FILE_CSV "${PROJECT_SOURCE_DIR}/model/Opc.Ua.Machinery.NodeSet2.csv"
    OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
    TARGET_SUFFIX "ids-machinery"
    ID_PREFIX "MACHINERY_"
)

ua_generate_nodeset(
    NAME "machinery"
    FILE "${PROJECT_SOURCE_DIR}/model/Opc.Ua.Machinery.NodeSet2.xml"
    OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
    # First NS0 Types, then other dependent namespaces
    DEPENDS_TYPES "UA_TYPES;UA_TYPES_DI"
    DEPENDS_NS "${open62541_NODESET_DIR}/Schema/Opc.Ua.NodeSet2.xml;${open62541_NODESET_DIR}/DI/Opc.Ua.Di.NodeSet2.xml"
)
# Workaround, as ua_generate_nodeset_and_datatypes expects a types-File.
file(TOUCH "${PROJECT_SOURCE_DIR}/src_generated/types_machinery_generated.h")

# ua_generate_nodeset_and_datatypes(
#     NAME "machinery"
#     FILE_NS "${PROJECT_SOURCE_DIR}/model/Opc.Ua.Machinery.NodeSet2.xml"
#     FILE_CSV "${PROJECT_SOURCE_DIR}/model/Opc.Ua.Machinery.NodeSet2.csv"
#     FILE_BSD "${PROJECT_SOURCE_DIR}/model/Opc.Ua.Machinery.NodeSet2.bsd"
#     OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
#     NAMESPACE_IDX 3
#     DEPENDS_TYPES "UA_TYPES_DI"
#     #DEPENDS_NS    "${UA_FILE_NS0}"
# )

ua_generate_nodeset_and_datatypes(
    NAME "industrial_automation"
    FILE_NS "${PROJECT_SOURCE_DIR}/model/Opc.Ua.Ia.NodeSet2.xml"
    FILE_CSV "${PROJECT_SOURCE_DIR}/model/Opc.Ua.Ia.NodeSet2.csv"
    FILE_BSD "${PROJECT_SOURCE_DIR}/model/Opc.Ua.Ia.NodeSet2.bsd"
    OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
    NAMESPACE_IDX 4
    DEPENDS_TYPES "UA_TYPES_IA;UA_TYPES_DI"
    DEPENDS di
    #DEPENDS_NS    "${UA_FILE_NS0}"
)

ua_generate_nodeset_and_datatypes(
    NAME "machinetool"
    FILE_NS "${PROJECT_SOURCE_DIR}/model/Opc.Ua.MachineTool.Nodeset2.xml"
    FILE_CSV "${PROJECT_SOURCE_DIR}/model/Opc.Ua.MachineTool.Nodeset2.csv"
    FILE_BSD "${PROJECT_SOURCE_DIR}/model/Opc.Ua.MachineTool.Nodeset2.bsd"
    OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
    NAMESPACE_IDX 5
    DEPENDS_TYPES "UA_TYPES;UA_TYPES_DI;UA_TYPES_MACHINERY;UA_TYPES_IA"
    DEPENDS di machinery industrial_automation
    #DEPENDS_NS    "${UA_FILE_NS0}"
)

add_library(machinetool_generated
    # Add dependend files to trigger the generation!
    src_generated/namespace_di_generated.c
    src_generated/types_di_generated.h
    src_generated/di_nodeids.h
    src_generated/namespace_machinery_generated.c
    src_generated/machinery.h
    src_generated/types_industrial_automation_generated.h
    src_generated/namespace_industrial_automation_generated.c
    src_generated/namespace_machinetool_generated.c
    src_generated/types_machinetool_generated.h
    src_generated/machinetool_nodeids.h
    src_generated/industrial_automation_nodeids.h
    )
target_link_libraries(machinetool_generated PUBLIC open62541::open62541)
set_target_properties(machinetool_generated PROPERTIES DEBUG_POSTFIX "d")

add_library(ExampleServerLib
    UmatiServerLib/ConvertSimpleValue.cpp
    UmatiServerLib/ConvertStructValue.cpp
    UmatiServerLib/BindHelper.cpp
    UmatiServerLib/NodesMaster.cpp
    UmatiServerLib/NodeValue.cpp
    UmatiServerLib/Util.cpp
    UmatiServerLib/Instantiation.cpp
    UmatiServerLib/OpcUaKeys.cpp
    UmatiServerLib/ServerHelper.cpp
    UmatiServerLib/StateMachine.cpp
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
    ExampleMachinetoolServer.cpp
    MachineTools/SimulatedMachineTool.cpp
    MachineTools/InstantiatedMachineTool.cpp
    MachineTools/FullMachineTool.cpp
    MachineTools/FullMachineToolDynamic.cpp
    MachineTools/BasicMachineTool.cpp
    MachineTools/MRMachineTool.cpp
    MachineTools/ShowcaseMachineTool.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
target_link_libraries(${PROJECT_NAME} PUBLIC ExampleServerLib)
target_link_libraries(${PROJECT_NAME} PUBLIC machinetool_generated)

add_subdirectory(tests)
include(CTest)


include(GenerateExportHeader)

set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${PROJECT_VERSION})


# export library (either static or shared depending on BUILD_SHARED_LIBS)
install(TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}Targets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
	)
