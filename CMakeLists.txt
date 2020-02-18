cmake_minimum_required(VERSION 3.0)
project(ExampleUmatiServer)

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

ua_generate_nodeset_and_datatypes(
		NAME "umati"
		FILE_NS "${PROJECT_SOURCE_DIR}/model/umati.xml"
		FILE_CSV "${PROJECT_SOURCE_DIR}/model/umati.csv"
		FILE_BSD "${PROJECT_SOURCE_DIR}/model/umati.bsd"
		OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src_generated"
		NAMESPACE_IDX 2
		#DEPENDS_TYPES "UA_TYPES"
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
		DEPENDS_TYPES "UA_TYPES;UA_TYPES_UMATI"
		DEPENDS_NS "${open62541_NODESET_DIR}/Schema/Opc.Ua.NodeSet2.xml;${PROJECT_SOURCE_DIR}/model/umati.xml"
)

add_library(umati_generated
		# Add dependend files to trigger the generation!
		src_generated/namespace_umati_generated.c
		src_generated/types_umati_generated.h
		src_generated/umati_nodeids.h
		src_generated/namespace_iswexample_generated.c
		src_generated/iswexample.h)
target_link_libraries(umati_generated PUBLIC open62541::open62541)
set_target_properties(umati_generated PROPERTIES DEBUG_POSTFIX "d")

add_library(ExampleServerLib
		BindValue.cpp
		BindStruct.cpp
		NodesMaster.cpp
		NodeValue.cpp
		SetupEvents.cpp
)
set_target_properties(ExampleServerLib PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ExampleServerLib PROPERTIES DEBUG_POSTFIX "d")
target_link_libraries(ExampleServerLib PUBLIC open62541::open62541)
target_link_libraries(ExampleServerLib PUBLIC Open62541Cpp::Open62541Cpp)
target_link_libraries(ExampleServerLib PUBLIC reflCpp::reflCpp)

add_executable(
		${PROJECT_NAME}
		ExampleUmatiServer.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
target_link_libraries(${PROJECT_NAME} PUBLIC ExampleServerLib)
target_link_libraries(${PROJECT_NAME} PUBLIC umati_generated)

add_subdirectory(tests)
include(CTest)
