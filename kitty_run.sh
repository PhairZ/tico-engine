#!/usr/bin/env sh

if [ -z "$1" ]; then
	echo "Error: No CMake target specified." >&2
	echo "Usage: $0 <cmake-target-name> [args...]" >&2
	exit 1
fi

TARGET_NAME="$1"
shift

BUILD_DIR="${BUILD_DIR:-build}"

if [ ! -d "$BUILD_DIR" ]; then
	echo "Error: Build directory '$BUILD_DIR' not found." >&2
	echo "Set BUILD_DIR environment variable if your build directory is named differently." >&2
	exit 1
fi

TARGET_BIN=$(find "$BUILD_DIR" -type f -name "$TARGET_NAME" -executable 2>/dev/null | head -n 1)

if [ -z "$TARGET_BIN" ]; then
	echo "Error: Could not locate executable for target '$TARGET_NAME' in '$BUILD_DIR'." >&2
	echo "Ensure the target has been built." >&2
	exit 1
fi

TARGET_BIN=$(realpath "$TARGET_BIN")
TARGET_DIR=$(dirname "$TARGET_BIN")

ORIGINAL_TTY=$(tty)

exec kitty \
	--config NONE \
	-o font_size=8 \
	-o "modify_font cell_height 90%" \
	--directory "$TARGET_DIR" \
	sh -c 'exec "$@" 2> '"$ORIGINAL_TTY" -- "$TARGET_BIN" "$@"
