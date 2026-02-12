#!/usr/bin/env bash

set -e

OUTPUT_NAME="FrameFlow-Installer.run"
CONFIG_PATH="config/config.xml"
PACKAGES_PATH="packages"

echo "====================================="
echo " Building FrameFlow Installer (QtIFW)"
echo "====================================="

if [ ! -f "$CONFIG_PATH" ]; then
    echo "Config file not found!"
    exit 1
fi

if [ ! -d "$PACKAGES_PATH" ]; then
    echo "Packages folder not found!"
    exit 1
fi

# Make sure binarycreator is in PATH or give full path
binarycreator --config "$CONFIG_PATH" --packages "$PACKAGES_PATH" "$OUTPUT_NAME"

chmod +x "$OUTPUT_NAME"

echo "Installer created successfully: $OUTPUT_NAME"
