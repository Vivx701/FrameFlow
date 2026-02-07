#!/usr/bin/env bash

set -euo pipefail

BUILD_DIR="build"
QT_PATH="/opt/Qt/6.9.2/gcc_64"
JOBS=$(nproc 2>/dev/null || echo 2)

BUILD_TESTS=OFF

# ---- Flatpak variables ----
APP_ID="io.github.Vivx701.FrameFlow"
FLATPAK_DIR="flatpak"
FLATPAK_BUILD_DIR="build-dir"
FLATPAK_REPO="repo"
FLATPAK_BUNDLE="frameflow.flatpak"
MANIFEST="io.github.vivx701.FrameFlow.yml"

show_help() {
  echo "Usage: ./build.sh [--configure] [--build] [--clean] [--deploy]"
  echo ""
  echo "Options:"
  echo "  --configure    Configure CMake and generate build files"
  echo "  --build        Build the project"
  echo "  --clean        Remove the build directory"
  echo "  --deploy       Build and generate Flatpak bundle"
  echo ""
  exit 0
}

DO_CONFIGURE=0
DO_BUILD=0
DO_CLEAN=0
DO_DEPLOY=0

# Parse arguments
while [[ $# -gt 0 ]]; do
  case "$1" in
    --configure) DO_CONFIGURE=1 ;;
    --build)     DO_BUILD=1 ;;
    --clean)     DO_CLEAN=1 ;;
    --deploy)    DO_DEPLOY=1 ;;
    -h|--help)   show_help ;;
    --tests)     BUILD_TESTS=ON ;;
    *)
      echo "Unknown option: $1"
      show_help
      ;;
  esac
  shift
done

# No args → show help
if [[ $DO_CONFIGURE -eq 0 && $DO_BUILD -eq 0 && $DO_CLEAN -eq 0 && $DO_DEPLOY -eq 0 ]]; then
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

  cmake -DCMAKE_PREFIX_PATH="$QT_PATH" -DBUILD_TESTS=${BUILD_TESTS} ..

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

# ---------------------- DEPLOY ----------------------
if [[ $DO_DEPLOY -eq 1 ]]; then
  echo "📦 Building Flatpak..."

  if [[ ! -d "$FLATPAK_DIR" ]]; then
    echo "❌ Flatpak directory not found."
    exit 1
  fi

  cd "$FLATPAK_DIR"

  echo "🔹 Step 1: Building Flatpak"
  flatpak-builder --force-clean "$FLATPAK_BUILD_DIR" "$MANIFEST"

  echo "🔹 Step 2: Creating Repository"
  flatpak-builder --repo="$FLATPAK_REPO" --force-clean "$FLATPAK_BUILD_DIR" "$MANIFEST"

  echo "🔹 Step 3: Generating Bundle"
  flatpak build-bundle "$FLATPAK_REPO" "$FLATPAK_BUNDLE" "$APP_ID"

  cd ..

  echo "✔️ Deploy complete."
  echo "Bundle created at: $FLATPAK_DIR/$FLATPAK_BUNDLE"
fi
