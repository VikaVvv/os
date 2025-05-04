cmake_minimum_required(VERSION 3.10)
project(lab4)

set(CMAKE_CXX_STANDARD 17)

# Основные исполняемые файлы
add_executable(Receiver Reciever.cpp)
add_executable(Sender Sender.cpp)

add_subdirectory( doctest)
include_directories( doctest)

add_subdirectory( tests)