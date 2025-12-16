cmake_minimum_required(VERSION 3.16)

project(frameflow-writer LANGUAGES CXX)

# Qt automation
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui)

# 🔥 Automatically collect sources & headers
file(GLOB_RECURSE FRAMEFLOW_WRITER_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE FRAMEFLOW_WRITER_HEADERS
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

# Library
add_library(frameflow-writer SHARED
    ${FRAMEFLOW_WRITER_SOURCES}
    ${FRAMEFLOW_WRITER_HEADERS}
)

# Public include path
target_include_directories(frameflow-writer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Qt link
target_link_libraries(frameflow-writer
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
)

# Export macro
target_compile_definitions(frameflow-writer
    PRIVATE FRAMEFLOW_WRITER_LIBRARY
)
