cmake_minimum_required(VERSION 3.17)
project(Chess_game)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include_directories(include)

set(SOURCE_FILES
        src/chess.cpp
        src/board.cpp
        )

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion)

add_executable(Sandbox main.cpp ${SOURCE_FILES})