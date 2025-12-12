cmake_minimum_required(VERSION 3.19)
project(FrameFlow LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(FrameFlow
    WIN32 MACOSX_BUNDLE
    main.cpp
    frameflow.cpp
    frameflow.h
    frameflow.ui
)

# collect translations (option: translations subdir exported list or root scans folder)
file(GLOB ROOT_TS_FILES "${CMAKE_SOURCE_DIR}/translations/*.ts")

qt_add_translations(
  TARGETS FrameFlow
  TS_FILES ${ROOT_TS_FILES}
)

target_link_libraries(FrameFlow
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS FrameFlow
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET FrameFlow
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
