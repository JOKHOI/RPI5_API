cmake_minimum_required(VERSION 3.10)  # Set the minimum version of CMake required

# Set the project name and version
project(MyProject VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add the executable
add_executable(MyExecutable main.c)

# Optionally, you can set some compiler flags
add_compile_options(-Wall -Wextra -pedantic)
