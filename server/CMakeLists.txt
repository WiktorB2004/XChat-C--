# CMakeLists.txt for server component of Network Chat Application

# Minimum version of CMake required
cmake_minimum_required(VERSION 3.27)

# Project name
project(NetworkChatServer)

# Set C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)

# Directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/server/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/server/include)
set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Source files
file(GLOB_RECURSE SERVER_SOURCES "${SOURCE_DIR}/*.cpp")

# Include directories
include_directories(${INCLUDE_DIR})

# Executable
add_executable(server ${SERVER_SOURCES})

# Set output directory
set_target_properties(server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)
