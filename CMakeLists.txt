cmake_minimum_required(VERSION 2.81)
project(pomodoro)
include_directories(include)
add_library(pomodoro
    include/SimplePomoTimer.h
    src/SimplePomoTimer.cpp
    )
add_executable(pomo src/pomo.cpp)
target_link_libraries(pomo pomodoro)
