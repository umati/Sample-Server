set(ADS_DIR $ENV{CMAKE_PREFIX_PATH} CACHE PATH "Path to directory 'cmake' of a build SDK")

find_library(ADS_LIB ads HINTS ${ADS_DIR})
if ("${ADS_LIB}" STREQUAL "ADS_LIB-NOTFOUND")
    message(SEND_ERROR "### Could not found CPP_MOSQUITTO, please specify CMAKE_PREFIX_PATH")
else ()
    message("### Found ADS_LIB library: '${ADS_LIB}'")
endif ()


find_path(ADS_INCLUDE AdsLib.h HINTS ${ADS_DIR} PATH_SUFFIXES ADS)
if ("${ADS_INCLUDE}" STREQUAL "ADS_INCLUDE-NOTFOUND")
    message(SEND_ERROR "### Could not found ADS header, please specify CMAKE_PREFIX_PATH")
else ()
    message("### Found ADS include: '${ADS_INCLUDE}'")
endif ()

add_library(ADS::ADS INTERFACE IMPORTED)
target_link_libraries(ADS::ADS INTERFACE ${ADS_LIB})
target_include_directories(ADS::ADS INTERFACE ${ADS_INCLUDE})