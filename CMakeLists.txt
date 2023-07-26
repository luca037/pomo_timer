cmake_minimum_required(VERSION 3.5)
project(pomodoro_timer)
include_directories(include)
add_library(pomodoro_timer
    include/SimplePomoTimer.h
    src/SimplePomoTimer.cpp
    )
add_executable(pomodoro src/pomo.cpp)
target_link_libraries(pomodoro pomodoro_timer)
