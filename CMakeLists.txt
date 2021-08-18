cmake_minimum_required(VERSION 3.16)
project(InfotechTask)

set(CMAKE_CXX_STANDARD 11)

add_executable(first first_app/main.cpp)
target_link_libraries(first -lws2_32)

add_executable(second second_app/main.cpp)
target_link_libraries(second -lws2_32)