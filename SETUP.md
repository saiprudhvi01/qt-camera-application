# Quick Setup Guide

This guide will help you get the Qt Camera Application running on your system quickly.

## Windows Quick Setup

### Option 1: Using Build Script (Recommended)
1. Run the automated build script:
   ```cmd
   scripts\build_windows.bat
   ```

### Option 2: Manual Setup
1. **Install Qt6**:
   - Download Qt6 from [Qt.io](https://www.qt.io/download)
   - Or use vcpkg: `vcpkg install qt6-base qt6-widgets`

2. **Install OpenCV**:
   - Download from [OpenCV.org](https://opencv.org/releases/)
   - Or use vcpkg: `vcpkg install opencv[core,imgproc,highgui,videoio]`

3. **Build**:
   ```cmd
   mkdir build
   cd build
   cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\Qt\6.x.x\mingw_64"
   cmake --build . --config Release
   ```

## Linux Quick Setup

### Option 1: Using Build Script (Recommended)
1. Make the script executable and run:
   ```bash
   chmod +x scripts/build_linux.sh
   ./scripts/build_linux.sh
   ```

### Option 2: Manual Setup
1. **Install dependencies**:
   ```bash
   sudo apt update
   sudo apt install qt6-base-dev qt6-tools-dev libopencv-dev cmake build-essential
   ```

2. **Build**:
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build . --config Release
   ```

## macOS Quick Setup

1. **Install dependencies**:
   ```bash
   brew install qt6 opencv cmake
   ```

2. **Build**:
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_PREFIX_PATH="/opt/homebrew/opt/qt6"
   cmake --build . --config Release
   ```

## Running the Application

After successful build:

**Windows**: `build\bin\QtCameraApp.exe`
**Linux/macOS**: `build/bin/QtCameraApp`

## Common Issues

### "Qt6 not found"
- Windows: Set `CMAKE_PREFIX_PATH` to your Qt6 installation directory
- Linux: Install `qt6-base-dev qt6-tools-dev`
- macOS: Ensure Qt6 is in CMAKE_PREFIX_PATH

### "OpenCV not found"
- Windows: Use vcpkg or set `OpenCV_DIR`
- Linux: Install `libopencv-dev`
- macOS: Install via Homebrew

### "Camera not accessible"
- Ensure no other applications are using the camera
- Check camera permissions in system settings
- Try running as administrator (Windows) or with sudo (Linux/macOS)

## Next Steps

Once the application is running:
1. Click "Play" to start the camera feed
2. Use the resolution dropdown to change quality
3. Test the playback controls (Pause, Resume, Forward, Rewind)

For detailed information, see the main [README.md](README.md). 