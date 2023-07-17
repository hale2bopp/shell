set(CURSES_USE_NCURSES TRUE)
find_package(Curses REQUIRED)
set(BINARY ${CMAKE_PROJECT_NAME}_tst)
include_directories(${CURSES_INCLUDE_DIR})

set(TEST_SOURCES shell-test.cpp main.cpp)
set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest ${CURSES_LIBRARIES})
