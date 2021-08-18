cmake_minimum_required(VERSION 3.16)
project(InfotechTask)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(first first_app/main.cpp)

add_executable(second second_app/main.cpp)

if(WIN32)
    target_link_libraries(first -lws2_32)
    target_link_libraries(second -lws2_32)
else()
    target_link_libraries(first -lpthread)
    target_link_libraries(second -lpthread)
endif()
