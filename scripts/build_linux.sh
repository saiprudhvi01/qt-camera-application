#!/bin/bash

echo "Building Qt Camera Application for Linux..."

# Check if dependencies are installed
check_dependency() {
    if ! command -v $1 &> /dev/null; then
        echo "ERROR: $1 is not installed or not in PATH"
        return 1
    fi
    return 0
}

echo "Checking dependencies..."

if ! check_dependency cmake; then
    echo "Please install CMake: sudo apt install cmake"
    exit 1
fi

if ! check_dependency g++; then
    echo "Please install build tools: sudo apt install build-essential"
    exit 1
fi

# Check for Qt6
if ! pkg-config --exists Qt6Core; then
    echo "Qt6 not found. Please install with:"
    echo "sudo apt install qt6-base-dev qt6-tools-dev"
    exit 1
fi

# Check for OpenCV
if ! pkg-config --exists opencv4; then
    echo "OpenCV not found. Please install with:"
    echo "sudo apt install libopencv-dev"
    exit 1
fi

echo "All dependencies found!"

# Create build directory
mkdir -p build
cd build

# Configure
echo "Configuring with CMake..."
if ! cmake ..; then
    echo "CMake configuration failed!"
    exit 1
fi

# Build
echo "Building..."
if cmake --build . --config Release; then
    echo ""
    echo "Build successful!"
    echo "Executable location: build/bin/QtCameraApp"
    echo ""
    echo "To run the application:"
    echo "  cd build/bin"
    echo "  ./QtCameraApp"
    echo ""
else
    echo ""
    echo "Build failed! Check the error messages above."
    echo ""
    exit 1
fi 