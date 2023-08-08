cmake_minimum_required(VERSION 3.5)
project(pomodoro_timer)

find_package(Curses REQUIRED)

include_directories(include ${CURSES_INCLUDE_DIR})

add_library(pomodoro_timer
    include/SimplePomoTimer.h
    src/SimplePomoTimer.cpp
    )

add_executable(pomodoro src/pomo.cpp)

target_link_libraries(pomodoro pomodoro_timer ${CURSES_LIBRARIES})
