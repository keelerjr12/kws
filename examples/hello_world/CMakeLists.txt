cmake_minimum_required(VERSION 3.26)
project(hello_world)

add_executable(hello_world main.cpp)

target_link_libraries(hello_world PUBLIC kws)
