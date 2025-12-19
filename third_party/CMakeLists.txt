cmake_minimum_required(VERSION 3.18)

# -------------------------------------------------
# third_party: FFmpeg (Windows only)
# -------------------------------------------------
if (NOT WIN32)
  message(STATUS "third_party: FFmpeg skipped (non-Windows)")
  return()
endif()

include(ExternalProject)

# -------------------------------------------------
# Layout (ALL under third_party/ffmpeg)
# -------------------------------------------------
set(FFMPEG_ROOT        ${CMAKE_CURRENT_LIST_DIR}/ffmpeg)
set(FFMPEG_SRC_DIR     ${FFMPEG_ROOT}/src)
set(FFMPEG_BUILD_DIR   ${FFMPEG_ROOT}/build)
set(FFMPEG_INSTALL_DIR ${FFMPEG_ROOT}/install)

# -------------------------------------------------
# FFmpeg source (LTS) - Download as ZIP from GitHub
# -------------------------------------------------
set(FFMPEG_TAG "n6.0")
set(FFMPEG_ZIP_URL "https://github.com/FFmpeg/FFmpeg/archive/refs/tags/${FFMPEG_TAG}.zip")

# -------------------------------------------------
# Toolchain detection
# -------------------------------------------------
if (MSVC)
  set(FFMPEG_TOOLCHAIN msvc)
  set(FFMPEG_LIB_SUFFIX lib)
else()
  set(FFMPEG_TOOLCHAIN mingw)
  set(FFMPEG_LIB_SUFFIX dll.a)
endif()

message(STATUS "third_party: FFmpeg toolchain = ${FFMPEG_TOOLCHAIN}")
message(STATUS "third_party: FFmpeg tag       = ${FFMPEG_TAG}")
message(STATUS "third_party: FFmpeg root      = ${FFMPEG_ROOT}")
message(STATUS "third_party: Download URL     = ${FFMPEG_ZIP_URL}")

# -------------------------------------------------
# Pre-create all required directories
# -------------------------------------------------
file(MAKE_DIRECTORY ${FFMPEG_ROOT})
file(MAKE_DIRECTORY ${FFMPEG_SRC_DIR})
file(MAKE_DIRECTORY ${FFMPEG_BUILD_DIR})
file(MAKE_DIRECTORY ${FFMPEG_INSTALL_DIR})

message(STATUS "third_party: FFmpeg directories created")

# -------------------------------------------------
# FFmpeg configure flags (shared build)
# -------------------------------------------------
set(FFMPEG_CONFIGURE_COMMON
  --prefix=${FFMPEG_INSTALL_DIR}
  --disable-programs
  --disable-doc
  --disable-debug
  --enable-shared
  --disable-static
  --enable-avcodec
  --enable-avformat
  --enable-avutil
  --enable-swscale
)

if (FFMPEG_TOOLCHAIN STREQUAL "mingw")
  set(FFMPEG_CONFIGURE_CMD
    ${FFMPEG_SRC_DIR}/configure
    --target-os=mingw32
    --arch=x86_64
    ${FFMPEG_CONFIGURE_COMMON}
  )
  set(FFMPEG_BUILD_CMD   make -j)
  set(FFMPEG_INSTALL_CMD make install)
else()
  set(FFMPEG_CONFIGURE_CMD
    cmd /c configure
    --toolchain=msvc
    --arch=x86_64
    ${FFMPEG_CONFIGURE_COMMON}
  )
  set(FFMPEG_BUILD_CMD   nmake)
  set(FFMPEG_INSTALL_CMD nmake install)
endif()

# -------------------------------------------------
# ExternalProject - Download ZIP (NO GIT!)
# -------------------------------------------------
# Use custom stamp directory to avoid permission issues
set(FFMPEG_STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/ffmpeg-stamps)
file(MAKE_DIRECTORY ${FFMPEG_STAMP_DIR})

# GitHub extracts to FFmpeg-n6.0/ but we want files directly in src/
set(FFMPEG_DOWNLOAD_DIR ${FFMPEG_ROOT}/download)

ExternalProject_Add(ffmpeg_ep
  PREFIX        ${FFMPEG_ROOT}
  SOURCE_DIR    ${FFMPEG_SRC_DIR}
  INSTALL_DIR   ${FFMPEG_INSTALL_DIR}
  STAMP_DIR     ${FFMPEG_STAMP_DIR}

  # Download ZIP from GitHub
  URL               ${FFMPEG_ZIP_URL}
  DOWNLOAD_DIR      ${FFMPEG_DOWNLOAD_DIR}
  DOWNLOAD_NO_EXTRACT TRUE
  DOWNLOAD_NO_PROGRESS OFF
  
  # Custom patch step to extract and move files to correct location
  # Run from FFMPEG_ROOT (parent directory) to avoid deleting current directory
  PATCH_COMMAND ${CMAKE_COMMAND} -E chdir ${FFMPEG_ROOT}
                    ${CMAKE_COMMAND} -E remove_directory src
        COMMAND ${CMAKE_COMMAND} -E chdir ${FFMPEG_ROOT}
                    ${CMAKE_COMMAND} -E tar xzf download/${FFMPEG_TAG}.zip
        COMMAND ${CMAKE_COMMAND} -E chdir ${FFMPEG_ROOT}
                    ${CMAKE_COMMAND} -E rename FFmpeg-${FFMPEG_TAG} src
  
  CONFIGURE_COMMAND ${FFMPEG_CONFIGURE_CMD}
  BUILD_COMMAND     ${FFMPEG_BUILD_CMD}
  INSTALL_COMMAND   ${FFMPEG_INSTALL_CMD}

  BUILD_IN_SOURCE 1
  
  # No update step needed
  UPDATE_COMMAND ""
)

# -------------------------------------------------
# Imported FFmpeg libraries
# -------------------------------------------------
foreach(lib avcodec avformat avutil swscale)
  add_library(ffmpeg::${lib} SHARED IMPORTED)
  add_dependencies(ffmpeg::${lib} ffmpeg_ep)

  set_target_properties(ffmpeg::${lib} PROPERTIES
    IMPORTED_IMPLIB
      ${FFMPEG_INSTALL_DIR}/lib/${lib}.${FFMPEG_LIB_SUFFIX}
    IMPORTED_LOCATION
      ${FFMPEG_INSTALL_DIR}/bin/${lib}-*.dll
    INTERFACE_INCLUDE_DIRECTORIES
      ${FFMPEG_INSTALL_DIR}/include
  )
endforeach()

# -------------------------------------------------
# Single public target exposed to the rest of the project
# -------------------------------------------------
add_library(ffmpeg_iface INTERFACE)
target_link_libraries(ffmpeg_iface INTERFACE
  ffmpeg::avcodec
  ffmpeg::avformat
  ffmpeg::avutil
  ffmpeg::swscale
)