#!/usr/bin/env bash

set -euo pipefail

BUILD_DIR="build"
QT_PATH="/opt/Qt/6.9.2/gcc_64"
JOBS=$(nproc 2>/dev/null || echo 2)

show_help() {
  echo "Usage: ./build.sh [--configure] [--build] [--clean]"
  echo ""
  echo "Options:"
  echo "  --configure    Configure CMake and generate build files"
  echo "  --build        Build the project"
  echo "  --clean        Remove the build directory"
  echo ""
  exit 0
}

DO_CONFIGURE=0
DO_BUILD=0
DO_CLEAN=0

# Parse arguments
while [[ $# -gt 0 ]]; do
  case "$1" in
    --configure) DO_CONFIGURE=1 ;;
    --build)     DO_BUILD=1 ;;
    --clean)     DO_CLEAN=1 ;;
    -h|--help)   show_help ;;
    *)
      echo "Unknown option: $1"
      show_help
      ;;
  esac
  shift
done

# No args → show help
if [[ $DO_CONFIGURE -eq 0 && $DO_BUILD -eq 0 && $DO_CLEAN -eq 0 ]]; then
  show_help
fi

# ---------------------- CLEAN ----------------------
if [[ $DO_CLEAN -eq 1 ]]; then
  echo "🧹 Cleaning build directory: $BUILD_DIR"
  rm -rf "$BUILD_DIR"
  echo "✔️ Clean complete."
fi

# -------------------- CONFIGURE --------------------
if [[ $DO_CONFIGURE -eq 1 ]]; then
  echo "⚙️ Configuring project..."

  mkdir -p "$BUILD_DIR"
  cd "$BUILD_DIR"

  cmake -DCMAKE_PREFIX_PATH="$QT_PATH" ..

  cd ..
  echo "✔️ Configure complete."
fi

# ---------------------- BUILD ----------------------
if [[ $DO_BUILD -eq 1 ]]; then
  echo "🔨 Building project..."

  if [[ ! -d "$BUILD_DIR" ]]; then
    echo "❌ Build directory not found. Run --configure first."
    exit 1
  fi

  cd "$BUILD_DIR"
  cmake --build . --parallel "$JOBS"

  cd ..
  echo "✔️ Build complete."
fi
