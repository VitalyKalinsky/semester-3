cmake_minimum_required(VERSION 3.16)
project(HelloQt CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.12.0/mingw_64")

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(HelloQt
    main.cpp
    MyWidget.h
    MyWidget.cpp
)

target_link_libraries(HelloQt PRIVATE Qt6::Widgets)