# Root CMakeLists.txt — minimal delegation only
cmake_minimum_required(VERSION 3.19)

project(FrameFlow LANGUAGES CXX)

# -------------------------------------------------------
# Root-only policy:
#  - This root file does NOT find Qt or other deps.
#  - Each subdirectory is responsible for its own find_package() / pkg_check_modules().
#  - This file only orders and adds subdirectories.
# -------------------------------------------------------

# Option to enable tests
option(BUILD_TESTS "Enable building tests" OFF)

# List the canonical subfolders and the order in which they should be added.
# Add new module folders here if you want them considered by default.
set(FRAMEFLOW_ORDERED_SUBDIRS
  frameflow-writer
  third_party
  translations
  app
  tests
)

# Helper: add a subdir if it exists
function(maybe_add_subdirectory subdir)
  if(EXISTS "${CMAKE_SOURCE_DIR}/${subdir}/CMakeLists.txt")
    message(STATUS "Root: adding subdirectory '${subdir}'")
    add_subdirectory("${subdir}")
  else()
    message(VERBOSE "Root: skipping missing subdirectory '${subdir}'")
  endif()
endfunction()

# Add directories in the defined order
foreach(s ${FRAMEFLOW_ORDERED_SUBDIRS})
  if(s STREQUAL "tests")
    if(BUILD_TESTS)
      maybe_add_subdirectory(${s})
    else()
      message(VERBOSE "Root: tests disabled (BUILD_TESTS=OFF).")
    endif()
  else()
    maybe_add_subdirectory(${s})
  endif()
endforeach()

# Optional: convenience aggregate target
# Subprojects can register their top-level targets by appending to a GLOBAL property
# e.g. in a subproject: set_property(GLOBAL APPEND PROPERTY FRAMEFLOW_SUBPROJECT_TARGETS MyLib)
get_property(_registered_targets GLOBAL PROPERTY FRAMEFLOW_SUBPROJECT_TARGETS)
if(_registered_targets)
  add_custom_target(all-modules)
  foreach(t ${_registered_targets})
    add_dependencies(all-modules ${t})
  endforeach()
endif()

message(STATUS "Root CMake finished. Subdirs processed in fixed order.")
