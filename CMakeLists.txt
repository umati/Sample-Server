# MIT License
#
# Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
# Copyright 2021 (c) Götz Görisch, VDW - Verein Deutscher Werkzeugmaschinenfabriken e.V.
# Copyright 2021 (c) Dennis Hezel, 3Yourmind GmbH
# Copyright 2021 (c) Hermann Leinweber, basysKom GmbH
# Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for umati and VDMA e.V.)
# Copyright 2022 (c) Patrick Moeller, 3Yourmind GmbH
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
project(SampleServer VERSION 1.1.1)

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

# Generate types and namespace for DI
ua_generate_nodeset_and_datatypes(
    NAME "di"
    FILE_NS "${UA_NODESET_DIR}/DI/Opc.Ua.Di.NodeSet2.xml"
    FILE_CSV "${UA_NODESET_DIR}/DI/Opc.Ua.Di.NodeIds.csv"
    FILE_BSD "${UA_NODESET_DIR}/DI/Opc.Ua.Di.Types.bsd"
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
    FILE_NS "${UA_NODESET_DIR}/Machinery/Opc.Ua.Machinery.NodeSet2.xml"
    FILE_CSV "${UA_NODESET_DIR}/Machinery/Opc.Ua.Machinery.NodeIds.csv"
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
    FILE_NS "${UA_NODESET_DIR}/IA/Opc.Ua.IA.NodeSet2.xml"
    FILE_CSV "${UA_NODESET_DIR}/IA/Opc.Ua.IA.NodeIds.csv"
    FILE_BSD "${UA_NODESET_DIR}/IA/Opc.Ua.IA.NodeSet2.bsd"
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
    FILE_NS "${UA_NODESET_DIR}/MachineTool/Opc.Ua.MachineTool.NodeSet2.xml"
    FILE_CSV "${UA_NODESET_DIR}/MachineTool/NodeIds.csv"
    FILE_BSD "${UA_NODESET_DIR}/MachineTool/Opc.Ua.MachineTool.NodeSet2.bsd"
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
   FILE_NS "${UA_NODESET_DIR}/Robotics/Opc.Ua.Robotics.NodeSet2.xml"
   FILE_CSV "${UA_NODESET_DIR}/Robotics/NodeIds.csv"
   FILE_BSD "${UA_NODESET_DIR}/Robotics/Opc.Ua.Robotics.Types.bsd"
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

ua_generate_nodeset_and_datatypes(
    NAME "machinery_result"
    FILE_NS "${UA_NODESET_DIR}/Machinery/Result/Opc.Ua.Machinery.Result.NodeSet2.xml"
    FILE_CSV "${UA_NODESET_DIR}/Machinery/Result/NodeIds.csv"
    FILE_BSD "${PROJECT_SOURCE_DIR}/model/MachineryResult/Opc.Ua.Machinery.Result.NodeSet2.bsd"
    NAMESPACE_MAP "8:http://opcfoundation.org/UA/Machinery/Result/"
    TARGET_PREFIX "${PROJECT_NAME}"
    OUTPUT_DIR "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}"
)

add_library(ua_gen_machinery_result)
target_sources(
    ua_gen_machinery_result
    PRIVATE # cmake-format: sort
            "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}/machinery_result_nodeids.h"
            ${UA_NODESET_MACHINERY_RESULT_HEADERS}
            ${UA_NODESET_MACHINERY_RESULT_SOURCES}
            ${UA_TYPES_MACHINERY_RESULT_HEADERS}
            ${UA_TYPES_MACHINERY_RESULT_SOURCES}
)
target_link_libraries(ua_gen_machinery_result PUBLIC open62541::open62541)

ua_generate_nodeset_and_datatypes(
    NAME "gms"
    FILE_NS "${UA_NODESET_DIR}/GMS/opc.ua.gms.nodeset2.xml"
    FILE_CSV "${UA_NODESET_DIR}/GMS/opc.ua.gms.nodesids.csv"
    FILE_BSD "${UA_NODESET_DIR}/GMS/opc.ua.gms.types.bsd"
    NAMESPACE_MAP "9:http://opcfoundation.org/UA/GMS/"
    TARGET_PREFIX "${PROJECT_NAME}"
    OUTPUT_DIR "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}"
    DEPENDS di ia machinery_result machinetool machinery
)

add_library(ua_gen_gms)
target_sources(
    ua_gen_gms
    PRIVATE # cmake-format: sort
            "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}/gms_nodeids.h" ${UA_NODESET_GMS_HEADERS}
            ${UA_NODESET_GMS_SOURCES} ${UA_TYPES_GMS_HEADERS} ${UA_TYPES_GMS_SOURCES}
)
target_link_libraries(ua_gen_gms PUBLIC ua_gen_machinetool ua_gen_ia ua_gen_machinery ua_gen_machinery_result)

ua_generate_nodeset_and_datatypes(
    NAME "irdi"
    FILE_NS "${UA_NODESET_DIR}/PADIM/Opc.Ua.IRDI.NodeSet2.xml"
    TARGET_PREFIX "${PROJECT_NAME}"
    OUTPUT_DIR "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}"
)

add_library(ua_gen_irdi)
target_sources(
    ua_gen_irdi
    PRIVATE # cmake-format: sort
            ${UA_NODESET_IRDI_HEADERS} ${UA_NODESET_IRDI_SOURCES}
)
target_link_libraries(ua_gen_irdi PUBLIC open62541::open62541)

set(TMP_NSURI "http://opcfoundation.org/UA/PADIM/")
configure_file(cmake/empty.bsd.template "${CMAKE_BINARY_DIR}/model/Opc.Ua.PADIM.Types.bsd")

ua_generate_nodeset_and_datatypes(
    NAME "padim"
    FILE_NS "${UA_NODESET_DIR}/PADIM/Opc.Ua.PADIM.NodeSet2.xml"
    FILE_CSV "${UA_NODESET_DIR}/PADIM/Opc.Ua.PADIM.NodeIds.csv"
    FILE_BSD "${CMAKE_BINARY_DIR}/model/Opc.Ua.PADIM.Types.bsd"
    NAMESPACE_MAP "11:http://opcfoundation.org/UA/PADIM/"
    TARGET_PREFIX "${PROJECT_NAME}"
    OUTPUT_DIR "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}"
    DEPENDS di irdi
)

add_library(ua_gen_padim)
target_sources(
    ua_gen_padim
    PRIVATE # cmake-format: sort
            "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}/padim_nodeids.h" ${UA_NODESET_PADIM_HEADERS}
            ${UA_NODESET_PADIM_SOURCES} ${UA_TYPES_PADIM_HEADERS} ${UA_TYPES_PADIM_SOURCES}
)
target_link_libraries(ua_gen_padim PUBLIC ua_gen_di ua_gen_irdi)

ua_generate_nodeset_and_datatypes(
    NAME "process_value"
    FILE_NS "${PROJECT_SOURCE_DIR}/model/AdditiveManufacturing/Opc.Ua.Machinery.ProcessValues.xml"
    TARGET_PREFIX "${PROJECT_NAME}"
    OUTPUT_DIR "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}"
    DEPENDS di irdi padim
)

add_library(ua_gen_process_value)
target_sources(
    ua_gen_process_value
    PRIVATE # cmake-format: sort
            ${UA_NODESET_PROCESS_VALUE_HEADERS} ${UA_NODESET_PROCESS_VALUE_SOURCES}
)
target_link_libraries(ua_gen_process_value PUBLIC ua_gen_di ua_gen_irdi ua_gen_padim)

ua_generate_nodeset_and_datatypes(
    NAME "additivemanufacturing"
    FILE_NS "${PROJECT_SOURCE_DIR}/model/AdditiveManufacturing/Opc.Ua.AdditiveManufacturing.NodeSet2.xml"
    FILE_CSV "${PROJECT_SOURCE_DIR}/model/AdditiveManufacturing/Opc.Ua.AdditiveManufacturing.NodeSet2.csv"
    FILE_BSD "${PROJECT_SOURCE_DIR}/model/AdditiveManufacturing/Opc.Ua.AdditiveManufacturing.NodeSet2.bsd"
    NAMESPACE_MAP "13:http://opcfoundation.org/UA/AdditiveManufacturing/NodeSet2/"
    TARGET_PREFIX "${PROJECT_NAME}"
    OUTPUT_DIR "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}"
    DEPENDS di
            ia
            machinery
            machinetool
            irdi
            padim
            process_value
)

add_library(ua_gen_additivemanufacturing)
target_sources(
    ua_gen_additivemanufacturing
    PRIVATE # cmake-format: sort
            "${SAMPLESERVER_GENERATED_NODESET_AND_DATATYPES_OUTPUT_DIR}/additivemanufacturing_nodeids.h"
            ${UA_NODESET_ADDITIVEMANUFACTURING_HEADERS}
            ${UA_NODESET_ADDITIVEMANUFACTURING_SOURCES}
            ${UA_TYPES_ADDITIVEMANUFACTURING_HEADERS}
            ${UA_TYPES_ADDITIVEMANUFACTURING_SOURCES}
)
target_link_libraries(
    ua_gen_additivemanufacturing
    PUBLIC ua_gen_di
           ua_gen_ia
           ua_gen_machinery
           ua_gen_machinetool
           ua_gen_irdi
           ua_gen_padim
           ua_gen_process_value
)

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

add_executable(
    ${PROJECT_NAME}
    AdditiveManufacturing/BasicAMMachine.cpp
    AdditiveManufacturing/InstantiatedAMMachine.cpp
    AdditiveManufacturing/ShowcaseAMMachine.cpp
    GMS/BasicGMS.cpp
    GMS/FullGMS.cpp
    GMS/HexagonGlobal.cpp
    GMS/HexagonSim.cpp
    GMS/InstantiatedGMS.cpp
    GMS/OGPSmartScopeCNC500.cpp
    GMS/WenzelLH.cpp
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

#set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
#target_link_libraries(
#    ${PROJECT_NAME} PUBLIC SampleServerLib Configuration ua_gen_machinetool ua_gen_robotics ua_gen_woodworking
#                           ua_gen_gms
#)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC SampleServerLib
           Configuration
           ua_gen_machinetool
           ua_gen_robotics
           ua_gen_woodworking
           ua_gen_gms
           ua_gen_additivemanufacturing
)

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
