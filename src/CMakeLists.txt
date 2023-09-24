set(BINARY ${CMAKE_PROJECT_NAME})
set(SOURCES main.cpp 
    shell.cpp 
    shell.h 
    redirection.h
    shellSignal.cpp
    shellSignal.h
    shellDriver.cpp
    shellDriver.h)

# Add coverage flags for C and C++ source code
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

add_executable(${BINARY}_run ${SOURCES})
add_library(${BINARY}_lib STATIC ${SOURCES})

