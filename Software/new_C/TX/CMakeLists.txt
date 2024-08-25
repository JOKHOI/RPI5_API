cmake_minimum_required(VERSION 3.10)

# Project name and version
project(PFE_RPI_TX VERSION 1.0 LANGUAGES C)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add executable target with source files
add_executable(PFE_RPI_TX main.c)
