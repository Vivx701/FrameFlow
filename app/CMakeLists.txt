cmake_minimum_required(VERSION 3.19)
project(FrameFlow LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools Concurrent)

qt_standard_project_setup()


# collect translations (option: translations subdir exported list or root scans folder)
file(GLOB ROOT_TS_FILES "${CMAKE_SOURCE_DIR}/translations/*.ts")

file(GLOB_RECURSE APP_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE APP_HEADERS
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

file(GLOB_RECURSE APP_UIS
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/forms/*.ui
)

qt_add_resources(APP_RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources.qrc
)

qt_add_executable(FrameFlow
    WIN32 MACOSX_BUNDLE
    ${APP_SOURCES}
    ${APP_HEADERS}
    ${APP_UIS}
    main.cpp
    ${APP_RESOURCES}
)

set_target_properties(FrameFlow PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
    AUTOUIC_SEARCH_PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/forms
)

qt_add_translations(
  TARGETS FrameFlow
  TS_FILES ${ROOT_TS_FILES}
)

target_include_directories(FrameFlow
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../common
)

target_link_libraries(FrameFlow
    PRIVATE
        frameflow-writer
        Qt::Core
        Qt::Widgets
        Qt::Concurrent
)

include(GNUInstallDirs)

install(TARGETS FrameFlow
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(FLATPAK_BUILD)
    message(STATUS "Flatpak build enabled")

    install(FILES
        ${CMAKE_SOURCE_DIR}/flatpak/io.github.vivx701.FrameFlow.desktop
        DESTINATION share/applications
    )
    install(FILES
        ${CMAKE_SOURCE_DIR}/app/Resources/Icons/FrameFlow.png
        DESTINATION share/icons/hicolor/512x512/apps
        RENAME io.github.vivx701.FrameFlow.png
    )
    install(FILES
        ${CMAKE_SOURCE_DIR}/app/Resources/Icons/FrameFlow.svg
        DESTINATION share/icons/hicolor/scalable/apps
        RENAME io.github.vivx701.FrameFlow.svg
    )
    install(FILES
        ${CMAKE_SOURCE_DIR}/flatpak/io.github.vivx701.FrameFlow.metainfo.xml
        DESTINATION share/metainfo
    )
endif()

if(NOT FLATPAK_BUILD)
    qt_generate_deploy_app_script(
        TARGET FrameFlow
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
    )
    install(SCRIPT ${deploy_script})
endif()
