cmake_minimum_required(VERSION 3.0)
project(SampleServerLib_Tests)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

add_library(test_util_client OBJECT util/UAClient.cpp)
target_link_libraries(test_util_client PUBLIC open62541::open62541 Open62541Cpp::Open62541Cpp)

add_executable(ReadCertClient readCertClient/ReadCertClient.cpp)
target_link_libraries(ReadCertClient PUBLIC open62541::open62541 Open62541Cpp::Open62541Cpp)

# NO_CMAKE_SYSTEM_PATH prevents the usage of 'FindGTest.cmake' from the cmake installation. As this does not include
# GMock.
find_package(GTest REQUIRED NO_CMAKE_SYSTEM_PACKAGE_REGISTRY)

add_executable(Test_BindStruct Test_BindStruct.cpp)
target_link_libraries(Test_BindStruct PRIVATE SampleServerLib GTest::gtest_main)
add_test(NAME BindStruct COMMAND Test_BindStruct)
set_tests_properties(BindStruct PROPERTIES TIMEOUT 20)

add_executable(Test_BindNodeId Test_BindNodeId.cpp)
target_link_libraries(Test_BindNodeId PRIVATE SampleServerLib GTest::gtest_main)
add_test(NAME BindNodeId COMMAND Test_BindNodeId)
set_tests_properties(BindNodeId PROPERTIES TIMEOUT 20)

add_executable(Test_BindWrongDatatype Test_BindWrongDatatype.cpp)
target_link_libraries(Test_BindWrongDatatype PRIVATE SampleServerLib GTest::gtest_main test_util_client)
add_test(NAME BindWrongDatatype COMMAND Test_BindWrongDatatype)
set_tests_properties(BindWrongDatatype PROPERTIES TIMEOUT 20)

add_executable(Test_MTEvent Test_MTEvent.cpp)
target_link_libraries(Test_MTEvent PRIVATE SampleServerLib ua_gen_machinetool GTest::gtest_main)
add_test(NAME MTEvent COMMAND Test_MTEvent)
set_tests_properties(MTEvent PROPERTIES TIMEOUT 20)

add_executable(Test_TranslateBrowse Test_TranslateBrowse.cpp)
target_link_libraries(Test_TranslateBrowse PRIVATE SampleServerLib ua_gen_machinetool GTest::gtest_main)
add_test(NAME TranslateBrowse COMMAND Test_TranslateBrowse)
set_tests_properties(MTEvent PROPERTIES TIMEOUT 20)
