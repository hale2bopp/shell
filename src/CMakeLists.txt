set(CURSES_USE_NCURSES TRUE)
find_package(Curses REQUIRED)
set(BINARY ${CMAKE_PROJECT_NAME})
include_directories(${CURSES_INCLUDE_DIR})
set(SOURCES main.cpp shell.cpp shell.h)

add_executable(${BINARY}_run ${SOURCES})
add_library(${BINARY}_lib STATIC ${SOURCES})
target_link_libraries(${BINARY}_run ${CURSES_LIBRARIES})

