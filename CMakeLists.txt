###############################################################################
# NAME:             CMakeLists.txt
#
# AUTHOR:           Ethan D. Twardy <edtwardy@mtu.edu>
#
# DESCRIPTION:      CMake project configuration.
#
# CREATED:          03/27/2020
#
# LAST EDITED:      04/02/2020
###

cmake_minimum_required(VERSION 3.15.1)

project(SortMedia DESCRIPTION "Media Sorter")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -O3 --std=c++17")

include(./CMakeDefs.txt)

add_library(networking STATIC
    "${NETWORKING_SOURCES}"
)
target_include_directories(networking PRIVATE
    include
)

enable_testing()
add_subdirectory(test)

###############################################################################
