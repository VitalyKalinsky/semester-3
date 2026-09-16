cmake_minimum_required(VERSION 3.20.0)
set (CMAKE_CXX_STANDARD 17)
project(lab2 LANGUAGES CXX)

add_library(mylib STATIC 
    src/MyString.cpp 
    src/MyString.hpp 
    src/MyVector.cpp 
    src/MyVector.h
)

add_executable(lab2_exe 
    src/Main.cpp
)

target_link_libraries(lab2_exe mylib)
