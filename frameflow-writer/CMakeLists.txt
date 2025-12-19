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

# 📥 Automatically collect sources & headers
file(GLOB_RECURSE FRAMEFLOW_WRITER_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE FRAMEFLOW_WRITER_HEADERS
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

# Library (CREATE TARGET FIRST)
add_library(frameflow-writer SHARED
    ${FRAMEFLOW_WRITER_SOURCES}
    ${FRAMEFLOW_WRITER_HEADERS}
)

# Public include path
target_include_directories(frameflow-writer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../common
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Export macro
target_compile_definitions(frameflow-writer
    PRIVATE FRAMEFLOW_WRITER_LIBRARY
)

# ------------------------------------------
# FFmpeg linkage (platform-specific)
# ------------------------------------------
if (WIN32)
    # From third_party/CMakeLists.txt
    # Ensure FFmpeg is built before this target
    add_dependencies(frameflow-writer ffmpeg_ep)
    target_link_libraries(frameflow-writer PRIVATE ffmpeg_iface)
else()
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(FFMPEG REQUIRED
        libavcodec
        libavformat
        libavutil
        libswscale
    )

    target_include_directories(frameflow-writer PRIVATE ${FFMPEG_INCLUDE_DIRS})
    target_link_libraries(frameflow-writer PRIVATE ${FFMPEG_LIBRARIES})
endif()

# Qt link
target_link_libraries(frameflow-writer
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
)

if(FLATPAK_BUILD)
    message(STATUS "Flatpak build enabled")
    install(TARGETS frameflow-writer LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()