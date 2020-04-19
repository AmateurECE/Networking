###############################################################################
# NAME:             CMakeLists.txt
#
# AUTHOR:           Ethan D. Twardy <edtwardy@mtu.edu>
#
# DESCRIPTION:      CMake project configuration.
#
# CREATED:          03/27/2020
#
# LAST EDITED:      04/18/2020
###

cmake_minimum_required(VERSION 3.15.1)

project(SortMedia DESCRIPTION "Media Sorter")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 --std=c++17")
# Debugging flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -O0 --std=c++17")

find_package(OpenSSL REQUIRED)
string(REGEX MATCH "^[0-9]+\.([0-9]+)\..*" theMatch ${OPENSSL_VERSION})
if(${CMAKE_MATCH_1} LESS "1")
    message(FATAL_ERROR "OpenSSL version 1.1.0 or greater is required.")
endif()

include(./CMakeDefs.txt)

add_library(networking STATIC
    "${NETWORKING_SOURCES}"
)
target_include_directories(networking PRIVATE
    include
    "${OPENSSL_INCLUDE_DIR}"
)
target_link_libraries(networking "${OPENSSL_LIBRARIES}")

# enable_testing()
# add_subdirectory(test)

###############################################################################
