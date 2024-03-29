set(BINARY ${PROJECT_NAME})
set(STANDALONE_SOURCES    
	main.cpp
    shell.cpp 
    shell.h 
    redirection.h
    shellSignal.cpp
    shellSignal.h
    shellDriver.h
    shellDriver.cpp
    shellSchedulerDriver.cpp
    shellSchedulerDriver.h
    functionLib.cpp
    )

set(LIB_SOURCES    
    shell.cpp 
    shellSignal.cpp
    shellDriver.cpp
    shellSchedulerDriver.cpp
    functionLib.cpp
    )


# Add coverage flags for C and C++ source code
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

add_executable(${BINARY}_run ${STANDALONE_SOURCES})

target_link_libraries(${BINARY}_run scheduler_lib)


add_library(${BINARY}_lib STATIC ${LIB_SOURCES})

target_include_directories(${BINARY}_lib PUBLIC ./)

target_link_libraries(${BINARY}_lib scheduler_lib)
