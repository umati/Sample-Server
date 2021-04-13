
set(REFL_CPP_DIR ${CMAKE_SOURCE_DIR}/deps/refl-cpp/include/ CACHE PATH "Directory of refl-cpp")

find_path(REFL_CPP_INCLUDE refl.hpp ${REFL_CPP_DIR})

if("${REFL_CPP_INCLUDE}" STREQUAL "REFL_CPP_INCLUDE-NOTFOUND")
	message(SEND_ERROR "Could not find refl.hpp, please specify REFL_CPP_DIR")
else()
	message("Found refl.hpp in:'${REFL_CPP_INCLUDE}' ")
endif()

add_library(reflCpp::reflCpp INTERFACE IMPORTED)
target_include_directories(reflCpp::reflCpp INTERFACE ${REFL_CPP_INCLUDE})
target_compile_definitions(reflCpp::reflCpp INTERFACE NOMINMAX)