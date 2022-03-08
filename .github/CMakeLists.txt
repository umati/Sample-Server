# MIT License
#
# Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
# Copyright 2021 (c) Götz Görisch, VDW - Verein Deutscher Werkzeugmaschinenfabriken e.V.
# Copyright 2021 (c) Dennis Hezel, 3Yourmind GmbH
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
#

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
    URL "https://ftp.openbsd.org/pub/OpenBSD/LibreSSL/libressl-3.5.3.tar.gz"
    URL_HASH SHA256=3ab5e5eaef69ce20c6b170ee64d785b42235f48f2e62b095fca5d7b6672b8b28
    PATCH_COMMAND "${Patch_EXECUTABLE}" -p1 --forward -d ${CMAKE_CURRENT_BINARY_DIR}/libreSslSource <
                  ${CMAKE_CURRENT_SOURCE_DIR}/ciPatches/libressl-no_postfix.patch || echo "Patch returned non-zero"
    CMAKE_ARGS -DLIBRESSL_APPS:BOOL=0 -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
)

externalproject_add(
    open62541External
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/open62541
    CMAKE_ARGS -DUA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS:BOOL=ON -DUA_ENABLE_SUBSCRIPTIONS_EVENTS:BOOL=ON
               -DUA_NAMESPACE_ZERO:STRING=FULL -DUA_ENABLE_ENCRYPTION:STRING=LIBRESSL
               -DLIBRESSL_ROOT_DIR:PATH=${CMAKE_INSTALL_PREFIX} -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
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
