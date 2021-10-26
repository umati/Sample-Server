cmake_minimum_required(VERSION 3.15)

project(BuildWithDependencies VERSION 0.1)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

externalproject_add(
    GTestExternal
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/googletest
    CMAKE_ARGS -Dgtest_force_shared_crt:BOOL=ON -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
)

externalproject_add(
    JsonExternal
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/json
    CMAKE_ARGS -DJSON_BuildTests:BOOL=OFF -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
)

find_package(Patch REQUIRED)
# Patch command might be applied already, so errors need to be ignored.
externalproject_add(
    libreSSLExternal
    DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/libreSslDownload
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/libreSslSource
    URL "https://ftp.openbsd.org/pub/OpenBSD/LibreSSL/libressl-3.4.1.tar.gz"
    URL_HASH SHA256=107ceae6ca800e81cb563584c16afa36d6c7138fade94a2b3e9da65456f7c61c
    PATCH_COMMAND "${Patch_EXECUTABLE}" -p1 --forward -d ${CMAKE_CURRENT_BINARY_DIR}/libreSslSource <
                  ${CMAKE_CURRENT_SOURCE_DIR}/ciPatches/libressl-no_postfix.patch || echo "Patch returned non-zero"
    CMAKE_ARGS -DLIBRESSL_APPS:BOOL=0 -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
)

externalproject_add(
    open62541External
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/open62541
    CMAKE_ARGS -DUA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS:BOOL=ON -DUA_ENABLE_SUBSCRIPTIONS_EVENTS:BOOL=ON
               -DUA_NAMESPACE_ZERO:STRING=FULL -DUA_ENABLE_ENCRYPTION:STRING=LIBRESSL -DLIBRESSL_ROOT_DIR:PATH=${CMAKE_INSTALL_PREFIX}
               -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
    DEPENDS libreSSLExternal
)

externalproject_add(
    open62541CppExternal
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/open62541Cpp
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
    DEPENDS open62541External GTestExternal
)

externalproject_add(
    Sample-Server
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
    DEPENDS open62541CppExternal GTestExternal JsonExternal
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/SampleServer-build
)
