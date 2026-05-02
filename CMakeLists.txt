cmake_minimum_required(VERSION 3.16)
project(texton)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(texton
    src/main.cpp
    src/buffer/gap_buffer.cpp
    src/buffer/piece_table.cpp
)

target_include_directories(texton PRIVATE ${CMAKE_SOURCE_DIR}/include)