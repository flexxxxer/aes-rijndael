cmake_minimum_required (VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)

project ("aes-rijndael")
add_subdirectory ("aes-impl")

add_executable (aes-rijndael "main.cpp")