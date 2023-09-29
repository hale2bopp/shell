cmake_minimum_required(VERSION 3.10)
project(shell)

set(CMAKE_CXX_STANDARD 14)
# include directories
include_directories(src)
include_directories(${CMAKE_SOURCE_DIR}/../lib/googletest/googletest/include)
include_directories(${CMAKE_SOURCE_DIR}/../lib/googletest/googlemock/include)

# add subdirectories
add_subdirectory(src)
add_subdirectory(test)

# set coverage flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
# cmake command 
# cmake .. -DCODE_COVERAGE=ON -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Debug
if(CMAKE_COMPILER_IS_GNUCXX)
include(CodeCoverage.cmake)
APPEND_COVERAGE_COMPILER_FLAGS()
SETUP_TARGET_FOR_COVERAGE_LCOV(NAME ${PROJECT_NAME}_coverage 
                               EXECUTABLE ${CMAKE_BINARY_DIR}/test/shell_tst
                               BASE_DIRECTORY ${CMAKE_SOURCE_DIR}/src )

endif()

