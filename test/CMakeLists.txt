set(BINARY ${CMAKE_PROJECT_NAME}_tst)

# set(TEST_SOURCES shell-test.cpp shell-test.h main.cpp shellDriverMock.cpp)
set(TEST_SOURCES shell-test.cpp shell-test.h main.cpp)
set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

# Add coverage flags for C and C++ test code
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")


target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest gmock)
