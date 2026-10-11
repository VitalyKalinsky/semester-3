cmake_minimum_required(VERSION 3.20.0)
set (CMAKE_CXX_STANDARD 20)
project(lab1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-fsanitize=address -fno-omit-frame-pointer -O1")

add_library(mylib STATIC 
    src/MultiSet.hpp
    src/MultiSet.cpp
)

add_executable(main 
    src/main.cpp
)

target_link_libraries(main mylib)

file(GENERATE 
     OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/run_test.sh"
     CONTENT "cat ../test_data.txt | ./main\n"
)