cmake_minimum_required(VERSION 3.15)
project(SampleServer VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(open62541 REQUIRED)
find_package(Open62541Cpp REQUIRED)
find_package(GTest REQUIRED NO_CMAKE_SYSTEM_PACKAGE_REGISTRY)

find_package(ReflCpp REQUIRED)
find_package(Threads REQUIRED)

if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # needed or cmake doesn't recognize dependencies of generated files
  set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR})
endif ()

# Generate types and namespace for DI
ua_generate_nodeset_and_datatypes(
  NAME "di"
  FILE_NS "${open62541_NODESET_DIR}/DI/Opc.Ua.Di.NodeSet2.xml"
  FILE_CSV "${open62541_NODESET_DIR}/DI/OpcUaDiModel.csv"
  FILE_BSD "${open62541_NODESET_DIR}/DI/Opc.Ua.Di.Types.bsd"
  NAMESPACE_MAP "2:http://opcfoundation.org/UA/DI/"
  OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
)

# Add also heder files to trigger the generation!
add_library(ua_gen_di
  src_generated/namespace_di_generated.c
  src_generated/types_di_generated.c
  src_generated/types_di_generated.h
  src_generated/di_nodeids.h
)
target_link_libraries(ua_gen_di PUBLIC open62541::open62541)

# See https://github.com/open62541/open62541/issues/3747
set(TMP_NSURI "http://opcfoundation.org/UA/Machinery/")
configure_file(cmake/empty.bsd.template "${CMAKE_BINARY_DIR}/model/Opc.Ua.Machinery.Types.bsd")

ua_generate_nodeset_and_datatypes(
  NAME "machinery"
  FILE_NS "${open62541_NODESET_DIR}/Machinery/Opc.Ua.Machinery.NodeSet2.xml"
  FILE_CSV "${open62541_NODESET_DIR}/Machinery/NodeIds.csv"
  FILE_BSD "${CMAKE_BINARY_DIR}/model/Opc.Ua.Machinery.Types.bsd"
  OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
  NAMESPACE_MAP "3:http://opcfoundation.org/UA/Machinery/"
  DEPENDS di
  )

add_library(ua_gen_machinery
  src_generated/namespace_machinery_generated.c
  src_generated/types_machinery_generated.c
  src_generated/types_machinery_generated.h
  src_generated/machinery_nodeids.h
)
target_link_libraries(ua_gen_machinery PUBLIC ua_gen_di)

ua_generate_nodeset_and_datatypes(
  NAME "ia"
  FILE_NS "${open62541_NODESET_DIR}/IA/Opc.Ua.IA.NodeSet2.xml"
  FILE_CSV "${open62541_NODESET_DIR}/IA/Opc.Ua.IA.NodeIds.csv"
  FILE_BSD "${PROJECT_SOURCE_DIR}/model/Opc.Ua.IA.fixed.bsd"
  OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
  NAMESPACE_MAP "4:http://opcfoundation.org/UA/IA/"
  DEPENDS di
)

add_library(ua_gen_ia
  src_generated/ia_nodeids.h
  src_generated/types_ia_generated.c
  src_generated/types_ia_generated.h
  src_generated/namespace_ia_generated.c
)
target_link_libraries(ua_gen_ia PUBLIC ua_gen_di)

ua_generate_nodeset_and_datatypes(
  NAME "machinetool"
  FILE_NS "${open62541_NODESET_DIR}/MachineTool/Opc.Ua.MachineTool.NodeSet2.xml"
  FILE_CSV "${open62541_NODESET_DIR}/MachineTool/NodeIds.csv"
  FILE_BSD "${PROJECT_SOURCE_DIR}/model/Opc.Ua.MachineTool.bsd"
  OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
  NAMESPACE_MAP "5:http://opcfoundation.org/UA/MachineTool/"
  DEPENDS di machinery ia
)

add_library(ua_gen_mt
  src_generated/namespace_machinetool_generated.c
  src_generated/types_machinetool_generated.c
  src_generated/types_machinetool_generated.h
  src_generated/machinetool_nodeids.h
)
target_link_libraries(ua_gen_mt PUBLIC ua_gen_ia)
target_link_libraries(ua_gen_mt PUBLIC ua_gen_machinery)

ua_generate_nodeset_and_datatypes(
  NAME "robotics"
  FILE_NS "${open62541_NODESET_DIR}/Robotics/Opc.Ua.Robotics.NodeSet2.xml"
  FILE_CSV "${open62541_NODESET_DIR}/Robotics/NodeIds.csv"
  FILE_BSD "${open62541_NODESET_DIR}/Robotics/Opc.Ua.Robotics.NodeSet2.bsd"
  OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
  NAMESPACE_MAP "6:http://opcfoundation.org/UA/Robotics/"
  DEPENDS di
)

add_library(ua_gen_robotic
  src_generated/namespace_robotics_generated.c
  src_generated/types_robotics_generated.c
  src_generated/types_robotics_generated.h
  src_generated/robotics_nodeids.h
)
target_link_libraries(ua_gen_robotic PUBLIC ua_gen_di)

add_library(SampleServerLib
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
set_target_properties(SampleServerLib PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(SampleServerLib PUBLIC open62541::open62541)
target_link_libraries(SampleServerLib PUBLIC Open62541Cpp::Open62541Cpp)
target_link_libraries(SampleServerLib PUBLIC reflCpp::reflCpp)
target_link_libraries(SampleServerLib PUBLIC Threads::Threads)
target_compile_definitions(SampleServerLib PUBLIC __STDC_WANT_LIB_EXT1__)

add_executable(
    ${PROJECT_NAME}
    SampleServer.cpp
    MachineTools/SimulatedInstance.cpp
    MachineTools/InstantiatedMachineTool.cpp
    MachineTools/FullMachineTool.cpp
    MachineTools/FullMachineToolDynamic.cpp
    MachineTools/BasicMachineTool.cpp
    MachineTools/MRMachineTool.cpp
    MachineTools/ShowcaseMachineTool.cpp
    Robotics/InstantiatedRobot.cpp
    Robotics/BasicRobot.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME} PUBLIC SampleServerLib)
target_link_libraries(${PROJECT_NAME} PUBLIC ua_gen_mt)
target_link_libraries(${PROJECT_NAME} PUBLIC ua_gen_robotic)

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
