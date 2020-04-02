###############################################################################
# NAME:             CMakeLists.txt
#
# AUTHOR:           Ethan D. Twardy <edtwardy@mtu.edu>
#
# DESCRIPTION:      Builds the tests for the project.
#
# CREATED:          09/13/2019
#
# LAST EDITED:      04/02/2020
###

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(NetworkingTests
    TestMain.cpp
    TCP/TCPIntegrationTest.cpp    
)

target_include_directories(NetworkingTests
    SYSTEM PRIVATE "${GTEST_INCLUDE_DIRS}"
)

target_include_directories(NetworkingTests
    PRIVATE ../include
)

target_link_libraries(NetworkingTests
    networking
    "${GTEST_BOTH_LIBRARIES}"
)

enable_testing()
gtest_discover_tests(NetworkingTests)

###############################################################################
