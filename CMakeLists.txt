# CMakeLists.txt for the project

# Minimum version of CMake required
cmake_minimum_required(VERSION 3.27)

# Include and execute server/ CMakeLists.txt
add_subdirectory(server)

# Include and execute client/ CMakeLists.txt
add_subdirectory(client)