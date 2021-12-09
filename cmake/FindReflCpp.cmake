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


set(REFL_CPP_DIR
    ${CMAKE_SOURCE_DIR}/deps/refl-cpp/include/
    CACHE PATH "Directory of refl-cpp"
)

find_path(REFL_CPP_INCLUDE refl.hpp ${REFL_CPP_DIR})

if("${REFL_CPP_INCLUDE}" STREQUAL "REFL_CPP_INCLUDE-NOTFOUND")
    message(SEND_ERROR "Could not find refl.hpp, please specify REFL_CPP_DIR")
else()
    message("Found refl.hpp in:'${REFL_CPP_INCLUDE}' ")
endif()

add_library(reflCpp::reflCpp INTERFACE IMPORTED)
target_include_directories(reflCpp::reflCpp INTERFACE ${REFL_CPP_INCLUDE})
target_compile_definitions(reflCpp::reflCpp INTERFACE NOMINMAX)
