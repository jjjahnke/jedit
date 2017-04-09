cmake_minimum_required(VERSION 3.7)
project(jedit)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main Config Terminal EditorIO)
add_executable(jedit ${SOURCE_FILES})