set(BINARY ${PROJECT_NAME}_tst)

set(TEST_SOURCES shell-test.cpp 
                 shell-test.h 
                 main.cpp 
                 shellDriverMock.h
                 command-history.cpp
                 )


set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

# Add coverage flags for C and C++ test code
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
message(CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
target_include_directories(${BINARY} PUBLIC 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../lib/googletest/googletest/include
    ${CMAKE_SOURCE_DIR}/../lib/googletest/googlemock/include
)
# Specify the path to the gmock and gtest library
link_directories(${CMAKE_SOURCE_DIR}/../lib/googletest/googletest)
link_directories(${CMAKE_SOURCE_DIR}/../lib/googletest/googlemock)
target_link_libraries(${BINARY} PUBLIC ${PROJECT_NAME}_lib gtest)
target_link_libraries(${BINARY} PUBLIC ${PROJECT_NAME}_lib gmock)
