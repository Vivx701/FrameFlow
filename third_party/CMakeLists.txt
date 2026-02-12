cmake_minimum_required(VERSION 3.18)

# -------------------------------------------------
# third_party: FFmpeg (Windows only) - PREBUILT
# -------------------------------------------------
if (NOT WIN32)
  message(STATUS "third_party: FFmpeg skipped (non-Windows)")
  return()
endif()

include(FetchContent)

# -------------------------------------------------
# Configuration
# -------------------------------------------------
set(FFMPEG_VERSION "6.0")
set(FFMPEG_URL "https://github.com/GyanD/codexffmpeg/releases/download/6.0/ffmpeg-6.0-full_build-shared.zip")

# -------------------------------------------------
# Paths
# -------------------------------------------------
set(FFMPEG_ROOT ${CMAKE_CURRENT_LIST_DIR}/ffmpeg)

# -------------------------------------------------
# Check if FFmpeg is already downloaded
# -------------------------------------------------
set(FFMPEG_MARKER_FILE "${FFMPEG_ROOT}/.ffmpeg_ready")

if(NOT EXISTS ${FFMPEG_MARKER_FILE})
  message(STATUS "FFmpeg: Not found, downloading...")

  # Clean up any partial downloads
  if(EXISTS ${FFMPEG_ROOT})
    message(STATUS "FFmpeg: Cleaning up existing folder")
    file(REMOVE_RECURSE ${FFMPEG_ROOT})
  endif()

  # Create directory
  file(MAKE_DIRECTORY ${FFMPEG_ROOT})

  # Download and extract
  message(STATUS "FFmpeg: Downloading from ${FFMPEG_URL}")
  message(STATUS "FFmpeg: This may take a few minutes...")

  FetchContent_Declare(
    ffmpeg_prebuilt
    URL ${FFMPEG_URL}
    SOURCE_DIR ${FFMPEG_ROOT}
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  )

  FetchContent_MakeAvailable(ffmpeg_prebuilt)

  # Create marker file to indicate successful download
  file(WRITE ${FFMPEG_MARKER_FILE} "FFmpeg ${FFMPEG_VERSION} downloaded successfully")
  message(STATUS "FFmpeg: Download and extraction complete!")
else()
  message(STATUS "FFmpeg: Using cached binaries from ${FFMPEG_ROOT}")
endif()

# -------------------------------------------------
# Locate FFmpeg directories
# -------------------------------------------------
set(FFMPEG_BASE "")

# Check if files are directly in root
if(EXISTS "${FFMPEG_ROOT}/bin" AND EXISTS "${FFMPEG_ROOT}/lib" AND EXISTS "${FFMPEG_ROOT}/include")
  set(FFMPEG_BASE ${FFMPEG_ROOT})
else()
  # Look for subdirectory containing the files
  file(GLOB FFMPEG_SUBDIRS "${FFMPEG_ROOT}/*")
  foreach(subdir ${FFMPEG_SUBDIRS})
    if(IS_DIRECTORY ${subdir})
      if(EXISTS "${subdir}/bin" AND EXISTS "${subdir}/lib" AND EXISTS "${subdir}/include")
        set(FFMPEG_BASE ${subdir})
        break()
      endif()
    endif()
  endforeach()
endif()

# If still not found, error out with helpful message
if(NOT FFMPEG_BASE)
  message(FATAL_ERROR
    "FFmpeg: Could not locate bin/lib/include directories in ${FFMPEG_ROOT}\n"
    "Try deleting ${FFMPEG_ROOT} and running configure again."
  )
endif()

message(STATUS "FFmpeg: Located at ${FFMPEG_BASE}")

# -------------------------------------------------
# Set FFmpeg paths
# -------------------------------------------------
set(FFMPEG_BIN_DIR "${FFMPEG_BASE}/bin")
set(FFMPEG_LIB_DIR "${FFMPEG_BASE}/lib")
set(FFMPEG_INCLUDE_DIR "${FFMPEG_BASE}/include")

message(STATUS "FFmpeg: bin = ${FFMPEG_BIN_DIR}")
message(STATUS "FFmpeg: lib = ${FFMPEG_LIB_DIR}")
message(STATUS "FFmpeg: include = ${FFMPEG_INCLUDE_DIR}")

# -------------------------------------------------
# Set cache variables (accessible to other CMakeLists)
# -------------------------------------------------
set(FFMPEG_BIN_DIR "${FFMPEG_BIN_DIR}" CACHE PATH "FFmpeg bin directory" FORCE)
set(FFMPEG_LIB_DIR "${FFMPEG_LIB_DIR}" CACHE PATH "FFmpeg lib directory" FORCE)
set(FFMPEG_INCLUDE_DIR "${FFMPEG_INCLUDE_DIR}" CACHE PATH "FFmpeg include directory" FORCE)

# -------------------------------------------------
# Detect library extension
# -------------------------------------------------
if(MSVC)
  set(FFMPEG_LIB_EXT "lib")
else()
  # MinGW - check what's available
  file(GLOB MINGW_DLL_A "${FFMPEG_LIB_DIR}/*.dll.a")
  if(MINGW_DLL_A)
    set(FFMPEG_LIB_EXT "dll.a")
  else()
    set(FFMPEG_LIB_EXT "lib")
  endif()
endif()

message(STATUS "FFmpeg: Library extension = ${FFMPEG_LIB_EXT}")

# -------------------------------------------------
# Create imported targets for each FFmpeg library
# -------------------------------------------------
set(FFMPEG_LIBRARIES avcodec avformat avutil swscale)

foreach(lib ${FFMPEG_LIBRARIES})
  # Find library file
  file(GLOB LIB_FILES
    "${FFMPEG_LIB_DIR}/${lib}.${FFMPEG_LIB_EXT}"
    "${FFMPEG_LIB_DIR}/lib${lib}.${FFMPEG_LIB_EXT}"
  )

  # Find DLL file
  file(GLOB DLL_FILES
    "${FFMPEG_BIN_DIR}/${lib}-*.dll"
    "${FFMPEG_BIN_DIR}/lib${lib}-*.dll"
    "${FFMPEG_BIN_DIR}/${lib}.dll"
  )

  if(LIB_FILES)
    list(GET LIB_FILES 0 LIB_FILE)
  else()
    message(WARNING "FFmpeg: Could not find import library for ${lib}")
    continue()
  endif()

  if(DLL_FILES)
    list(GET DLL_FILES 0 DLL_FILE)
  else()
    message(WARNING "FFmpeg: Could not find DLL for ${lib}")
    continue()
  endif()

  # Create imported library target
  add_library(ffmpeg::${lib} SHARED IMPORTED GLOBAL)

  set_target_properties(ffmpeg::${lib} PROPERTIES
    IMPORTED_IMPLIB "${LIB_FILE}"
    IMPORTED_LOCATION "${DLL_FILE}"
    INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIR}"
  )

  message(STATUS "FFmpeg: ✓ ${lib}")
  message(STATUS "  → LIB: ${LIB_FILE}")
  message(STATUS "  → DLL: ${DLL_FILE}")
endforeach()

# -------------------------------------------------
# Create unified interface target
# -------------------------------------------------
add_library(ffmpeg INTERFACE)

target_link_libraries(ffmpeg INTERFACE
  ffmpeg::avcodec
  ffmpeg::avformat
  ffmpeg::avutil
  ffmpeg::swscale
)

target_include_directories(ffmpeg INTERFACE
  ${FFMPEG_INCLUDE_DIR}
)

# -------------------------------------------------
# Create compatibility aliases
# -------------------------------------------------
# Alias for existing code that uses ffmpeg_iface
add_library(ffmpeg_iface ALIAS ffmpeg)

# Dummy target for existing code that depends on ffmpeg_ep
add_custom_target(ffmpeg_ep)

message(STATUS "FFmpeg: Configuration complete!")
message(STATUS "FFmpeg: Link your target with 'ffmpeg_iface'")
