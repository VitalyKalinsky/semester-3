cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 20)
project(lab3 LANGUAGES CXX)

add_library(mylib STATIC 
    src/MyString.cpp 
    src/MyString.hpp
    src/point.hpp 
)

add_executable(lab3 
    src/Lab4.cpp
)

target_link_libraries(lab3 mylib)