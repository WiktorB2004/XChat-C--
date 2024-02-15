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

# Include directories
include_directories(${INCLUDE_DIR})
include_directories(${LIBWEBSOCKETS_INCLUDE_DIRS})

# Source files
set(CLIENT_SOURCES
    ${SOURCE_DIR}/client.cpp
    ${SOURCE_DIR}/client_connection.cpp
    ${SOURCE_DIR}/custom_qelements.cpp
    ${SOURCE_DIR}/chat.cpp
    ${SOURCE_DIR}/login.cpp
    ${SOURCE_DIR}/main.cpp
)

QT5_WRAP_CPP(MOC_FILES
    ${INCLUDE_DIR}/client.h
    ${INCLUDE_DIR}/app_windows.h
    ${INCLUDE_DIR}/server_connection.h
    ${INCLUDE_DIR}/custom_qelements.h
)

# Executable
add_executable(client ${CLIENT_SOURCES} ${MOC_FILES})

target_link_libraries(client PRIVATE Qt5::Core Qt5::Widgets)
target_link_libraries(client PRIVATE ${LIBWEBSOCKETS_LIBRARIES})
target_link_libraries(client PRIVATE pthread)

# Set output directory
set_target_properties(client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)
