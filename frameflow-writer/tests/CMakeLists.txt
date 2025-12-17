find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Test Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test Gui)

add_executable(frameflow-writer-test
    TestFrameFlowWriter.cpp
)

target_link_libraries(frameflow-writer-test
    PRIVATE
        frameflow-writer
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Test
        Qt${QT_VERSION_MAJOR}::Gui
)

add_test(
    NAME frameflow-writer
    COMMAND frameflow-writer-test
)

add_custom_command(
    TARGET frameflow-writer-test
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/SampleImages
            $<TARGET_FILE_DIR:frameflow-writer-test>/SampleImages
)