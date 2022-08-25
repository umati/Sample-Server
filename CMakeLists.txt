# MIT License
# 
# Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
# Copyright 2021 (c) Götz Görisch, VDW - Verein Deutscher Werkzeugmaschinenfabriken e.V.
# Copyright 2021 (c) Dennis Hezel, 3Yourmind GmbH
# Copyright 2021 (c) Hermann Leinweber, basysKom GmbH
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


cmake_minimum_required(VERSION 3.15)
project(SampleServer VERSION 0.3)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(open62541 REQUIRED)
find_package(Open62541Cpp REQUIRED)
find_package(GTest REQUIRED NO_CMAKE_SYSTEM_PACKAGE_REGISTRY)
find_package(nlohmann_json REQUIRED)

find_package(ReflCpp REQUIRED)
find_package(Threads REQUIRED)

if(NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # needed or cmake doesn't recognize dependencies of generated files
    set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR})
endif()

set(SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated")
set(open62541_NODESET_DIR "${PROJECT_SOURCE_DIR}/deps/open62541/deps/ua-nodeset/")

# Generate types and namespace for DI
ua_generate_nodeset_and_datatypes(
    NAME "di"
    FILE_NS "${open62541_NODESET_DIR}/DI/Opc.Ua.Di.NodeSet2.xml"
    FILE_CSV "${open62541_NODESET_DIR}/DI/OpcUaDiModel.csv"
    FILE_BSD "${open62541_NODESET_DIR}/DI/Opc.Ua.Di.Types.bsd"
    NAMESPACE_MAP "2:http://opcfoundation.org/UA/DI/"
    TARGET_PREFIX "${PROJECT_NAME}"
    OUTPUT_DIR "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}"
)

add_library(ua_gen_di)
target_sources(
    ua_gen_di
    PRIVATE # cmake-format: sort
            "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}/di_nodeids.h" ${UA_NODESET_DI_HEADERS}
            ${UA_NODESET_DI_SOURCES} ${UA_TYPES_DI_HEADERS} ${UA_TYPES_DI_SOURCES}
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
    NAMESPACE_MAP "3:http://opcfoundation.org/UA/Machinery/"
    TARGET_PREFIX "${PROJECT_NAME}"
    OUTPUT_DIR "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}"
    DEPENDS di
)

add_library(ua_gen_machinery)
target_sources(
    ua_gen_machinery
    PRIVATE # cmake-format: sort
            "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}/machinery_nodeids.h"
            ${UA_NODESET_MACHINERY_HEADERS}
            ${UA_NODESET_MACHINERY_SOURCES}
            ${UA_TYPES_MACHINERY_HEADERS}
            ${UA_TYPES_MACHINERY_SOURCES}
)
target_link_libraries(ua_gen_machinery PUBLIC ua_gen_di)

ua_generate_nodeset_and_datatypes(
    NAME "ia"
    FILE_NS "${open62541_NODESET_DIR}/IA/Opc.Ua.IA.NodeSet2.xml"
    FILE_CSV "${open62541_NODESET_DIR}/IA/Opc.Ua.IA.NodeIds.csv"
    FILE_BSD "${PROJECT_SOURCE_DIR}/model/Opc.Ua.IA.fixed.bsd"
    NAMESPACE_MAP "4:http://opcfoundation.org/UA/IA/"
    TARGET_PREFIX "${PROJECT_NAME}"
    OUTPUT_DIR "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}"
    DEPENDS di
)

add_library(ua_gen_ia)
target_sources(
    ua_gen_ia
    PRIVATE # cmake-format: sort
            "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}/ia_nodeids.h" ${UA_NODESET_IA_HEADERS}
            ${UA_NODESET_IA_SOURCES} ${UA_TYPES_IA_HEADERS} ${UA_TYPES_IA_SOURCES}
)
target_link_libraries(ua_gen_ia PUBLIC ua_gen_di)

ua_generate_nodeset_and_datatypes(
    NAME "machinetool"
    FILE_NS "${open62541_NODESET_DIR}/MachineTool/Opc.Ua.MachineTool.NodeSet2.xml"
    FILE_CSV "${open62541_NODESET_DIR}/MachineTool/NodeIds.csv"
    FILE_BSD "${PROJECT_SOURCE_DIR}/model/Opc.Ua.MachineTool.bsd"
    NAMESPACE_MAP "5:http://opcfoundation.org/UA/MachineTool/"
    TARGET_PREFIX "${PROJECT_NAME}"
    OUTPUT_DIR "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}"
    DEPENDS di machinery ia
)

add_library(ua_gen_machinetool)
target_sources(
    ua_gen_machinetool
    PRIVATE # cmake-format: sort
            "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}/machinetool_nodeids.h"
            ${UA_NODESET_MACHINETOOL_HEADERS}
            ${UA_NODESET_MACHINETOOL_SOURCES}
            ${UA_TYPES_MACHINETOOL_HEADERS}
            ${UA_TYPES_MACHINETOOL_SOURCES}
)
target_link_libraries(ua_gen_machinetool PUBLIC ua_gen_ia ua_gen_machinery)

ua_generate_nodeset_and_datatypes(
    NAME "robotics"
    FILE_NS "${open62541_NODESET_DIR}/Robotics/Opc.Ua.Robotics.NodeSet2.xml"
    FILE_CSV "${open62541_NODESET_DIR}/Robotics/NodeIds.csv"
    FILE_BSD "${open62541_NODESET_DIR}/Robotics/Opc.Ua.Robotics.Types.bsd"
    NAMESPACE_MAP "6:http://opcfoundation.org/UA/Robotics/"
    TARGET_PREFIX "${PROJECT_NAME}"
    OUTPUT_DIR "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}"
    DEPENDS di
)

add_library(ua_gen_robotics)
target_sources(
    ua_gen_robotics
    PRIVATE # cmake-format: sort
            "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}/robotics_nodeids.h"
            ${UA_NODESET_ROBOTICS_HEADERS}
            ${UA_NODESET_ROBOTICS_SOURCES}
            ${UA_TYPES_ROBOTICS_HEADERS}
            ${UA_TYPES_ROBOTICS_SOURCES}
)
target_link_libraries(ua_gen_robotics PUBLIC ua_gen_di)

ua_generate_nodeset_and_datatypes(
    NAME "woodworking"
    FILE_NS "${PROJECT_SOURCE_DIR}/model/Woodworking/Opc.Ua.Woodworking.NodeSet2.xml"
    FILE_CSV "${PROJECT_SOURCE_DIR}/model/Woodworking/Opc.Ua.Woodworking.NodeIds.csv"
    FILE_BSD "${PROJECT_SOURCE_DIR}/model/Woodworking/Opc.Ua.Woodworking.NodeSet2.bsd"
    NAMESPACE_MAP "7:http://opcfoundation.org/UA/Woodworking/"
    TARGET_PREFIX "${PROJECT_NAME}"
    OUTPUT_DIR "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}"
    DEPENDS machinery
)

add_library(ua_gen_woodworking)
target_sources(
    ua_gen_woodworking
    PRIVATE # cmake-format: sort
            "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}/woodworking_nodeids.h"
            ${UA_NODESET_WOODWORKING_HEADERS}
            ${UA_NODESET_WOODWORKING_SOURCES}
            ${UA_TYPES_WOODWORKING_HEADERS}
            ${UA_TYPES_WOODWORKING_SOURCES}
)
target_link_libraries(ua_gen_woodworking PUBLIC ua_gen_machinery)

add_library(
    SampleServerLib
    OpcUaTypes/ConstNodeId.cpp
    OpcUaTypes/EUInformation.cpp
    OpcUaTypes/LocalizedText.cpp
    UmatiServerLib/BindHelper.cpp
    UmatiServerLib/ConvertSimpleValue.cpp
    UmatiServerLib/ConvertStructValue.cpp
    UmatiServerLib/Instantiation.cpp
    UmatiServerLib/NodesMaster.cpp
    UmatiServerLib/NodeValue.cpp
    UmatiServerLib/OpcUaKeys.cpp
    UmatiServerLib/ServerHelper.cpp
    UmatiServerLib/StateMachine.cpp
    UmatiServerLib/Util.cpp
)
set_target_properties(SampleServerLib PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(SampleServerLib PUBLIC open62541::open62541)
target_link_libraries(SampleServerLib PUBLIC Open62541Cpp::Open62541Cpp)
target_link_libraries(SampleServerLib PUBLIC reflCpp::reflCpp)
target_link_libraries(SampleServerLib PUBLIC Threads::Threads)
target_compile_definitions(SampleServerLib PUBLIC __STDC_WANT_LIB_EXT1__)

add_library(Configuration Configuration/Configuration.cpp Configuration/Configuration_json.cpp)
target_link_libraries(Configuration PRIVATE nlohmann_json::nlohmann_json)

set(UA_ENABLE_PUBSUB)
set(UA_ENABLE_PUBSUB_MQTT)
set(UA_ENABLE_JSON_ENCODING)

add_executable(
    ${PROJECT_NAME}
    MachineTools/BasicMachineTool.cpp
    MachineTools/CNShowcaseMachineTool.cpp
    MachineTools/FullMachineTool.cpp
    MachineTools/FullMachineToolDynamic.cpp
    MachineTools/InstantiatedMachineTool.cpp
    MachineTools/MRMachineTool.cpp
    MachineTools/ShowcaseMachineTool.cpp
    MachineTools/SimulatedInstance.cpp
    Robotics/BasicRobot.cpp
    Robotics/InstantiatedRobot.cpp
    SampleServer.cpp
    Woodworking/BasicWoodworking.cpp
    Woodworking/FullWoodworking.cpp
    Woodworking/InstantiatedWoodworking.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(
    ${PROJECT_NAME} PUBLIC SampleServerLib Configuration ua_gen_machinetool ua_gen_robotics ua_gen_woodworking
)
target_compile_definitions(${PROJECT_NAME} PRIVATE UA_ENABLE_PUBSUB=ON UA_ENABLE_PUBSUB_MQTT=ON UA_ENABLE_MQTT_TLS=ON)

add_subdirectory(tests)
include(CTest)

include(GenerateExportHeader)

set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${PROJECT_VERSION})

# export library (either static or shared depending on BUILD_SHARED_LIBS)
install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES
    DESTINATION include
)
