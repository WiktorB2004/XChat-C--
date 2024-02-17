# CMakeLists.txt for client component of Network Chat Application

# Minimum version of CMake required
cmake_minimum_required(VERSION 3.27)

# Project name
project(NetworkChatClient LANGUAGES CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets WebSockets REQUIRED)
find_package(Libwebsockets REQUIRED)

# Directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Include directories
include_directories(${INCLUDE_DIR})
include_directories(${LIBWEBSOCKETS_INCLUDE_DIRS})

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.h")

# Executable
qt_add_executable(client ${SOURCES})

target_link_libraries(client PUBLIC Qt::Core Qt::Widgets Qt::WebSockets)
target_link_libraries(client PUBLIC ${LIBWEBSOCKETS_LIBRARIES})
target_link_libraries(client PUBLIC pthread)

# Set output directory
set_target_properties(client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)
