cmake_minimum_required(VERSION 3.9)
project(Five_philosophers)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread ")

add_executable(Five_philosophers main.cpp Philosopher.cpp Philosopher.h DrawGui.cpp DrawGui.h)

target_link_libraries(Five_philosophers curses)