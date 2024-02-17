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
set(CLIENT_SOURCES
    ${INCLUDE_DIR}/chat_connection.h
    ${INCLUDE_DIR}/custom_qelements.h
    ${INCLUDE_DIR}/app_windows.h
    ${SOURCE_DIR}/chat_connection.cpp
    ${SOURCE_DIR}/custom_qelements.cpp
    ${SOURCE_DIR}/chat.cpp
    ${SOURCE_DIR}/login.cpp
    ${SOURCE_DIR}/main.cpp
)

# Executable
qt_add_executable(client ${CLIENT_SOURCES})

target_link_libraries(client PUBLIC Qt::Core Qt::Widgets Qt::WebSockets)
target_link_libraries(client PUBLIC ${LIBWEBSOCKETS_LIBRARIES})
target_link_libraries(client PUBLIC pthread)

# Set output directory
set_target_properties(client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)
