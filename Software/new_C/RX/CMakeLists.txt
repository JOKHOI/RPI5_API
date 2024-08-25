cmake_minimum_required(VERSION 3.10)

# Project name and the type of project
project(PFE_RPI_RX VERSION 1.0 LANGUAGES C)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add executable target with source files
add_executable(PFE_RPI_RX main.c)

# Optionally, set the compile options.
# target_compile_options(PFE_RPI_RX PRIVATE -Wall -Wextra)
