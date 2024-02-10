# CMakeLists.txt for client component of Network Chat Application

# Minimum version of CMake required
cmake_minimum_required(VERSION 3.27)

# Project name
project(NetworkChatClient)

# Set C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)

# Directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Source files
file(GLOB_RECURSE CLIENT_SOURCES "${SOURCE_DIR}/*.cpp")

# Include directories
include_directories(${INCLUDE_DIR})

# Executable
add_executable(client ${CLIENT_SOURCES})

# Set output directory
set_target_properties(client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)
