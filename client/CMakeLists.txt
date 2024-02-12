# CMakeLists.txt for client component of Network Chat Application

# Minimum version of CMake required
cmake_minimum_required(VERSION 3.27)

# Project name
project(NetworkChatClient)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt5 REQUIRED COMPONENTS Core Widgets REQUIRED)
find_package(Libwebsockets REQUIRED)

# Directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Source files
file(GLOB_RECURSE CLIENT_SOURCES "${SOURCE_DIR}/*.cpp")

# Include directories
include_directories(${INCLUDE_DIR})
include_directories(${LIBWEBSOCKETS_INCLUDE_DIRS})

# Executable
add_executable(client ${CLIENT_SOURCES})

target_link_libraries(client PRIVATE Qt5::Core Qt5::Widgets)
target_link_libraries(client PRIVATE ${LIBWEBSOCKETS_LIBRARIES})
target_link_libraries(client PRIVATE pthread)

# Set output directory
set_target_properties(client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)
