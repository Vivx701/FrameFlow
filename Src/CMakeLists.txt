cmake_minimum_required(VERSION 3.5)

project(FrameFlow LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(XKB REQUIRED xkbcommon)
    include_directories(${XKB_INCLUDE_DIRS})
    link_directories(${XKB_LIBRARY_DIRS})
endif()

set(UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Forms)

set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/exportdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/giffile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/htmlgallery.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/iconthememanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/imagedelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/imagemodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/imagesprite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/outputfilefactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/pdffile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/progressdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/settingsdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/videofile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/FrameFlowSplashScreen.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/IOutputFile.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/Strings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/Types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/aboutDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/exportdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/giffile.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/htmlgallery.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/iconthememanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/imagedelegate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/imagemodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/imagesprite.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/outputfilefactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/pdffile.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/progressdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/settingsdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/videofile.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/gif.h
    ${UI_DIR}/exportdialog.ui
    ${UI_DIR}/mainwindow.ui
    ${UI_DIR}/progressdialog.ui
    ${UI_DIR}/settingsdialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources.qrc
)

add_executable(FrameFlow
    ${PROJECT_SOURCES}
)

target_include_directories(FrameFlow PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
    ${CMAKE_CURRENT_BINARY_DIR}
)

set_target_properties(FrameFlow PROPERTIES
    AUTOUIC_SEARCH_PATHS ${UI_DIR}
)

target_link_libraries(FrameFlow PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    avformat
    avcodec
    avutil
    swscale
)

if(UNIX AND NOT APPLE)
    target_link_libraries(FrameFlow PRIVATE ${XKB_LIBRARIES})
endif()

if(WIN32)
    set_target_properties(FrameFlow PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Copy QSS files to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Theme/Dark.qss
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Resources/Theme)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Theme/Light.qss
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Resources/Theme)

# Install rules
if(WIN32)
    install(TARGETS FrameFlow DESTINATION bin)
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/Resources/Theme/Dark.qss
        ${CMAKE_CURRENT_BINARY_DIR}/Resources/Theme/Light.qss
        DESTINATION bin/Resources/Theme
    )
else()
    install(TARGETS FrameFlow DESTINATION bin)
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/Resources/Theme/Dark.qss
        ${CMAKE_CURRENT_BINARY_DIR}/Resources/Theme/Light.qss
        DESTINATION share/FrameFlow/Resources/Theme
    )
endif()

# Print some debug information
message(STATUS "Project source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Project binary dir: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "Project sources: ${PROJECT_SOURCES}")
