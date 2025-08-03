# Qt Camera Application

A Qt-based C++ application that displays live camera feeds using OpenCV with comprehensive playback controls and resolution settings.

## Features

- **Live Camera Feed**: Display real-time camera feed in a resizable window
- **Playback Controls**: 
  - Play: Start the camera feed
  - Pause: Freeze the current frame
  - Resume: Continue after pause
  - Forward: Skip frames forward
  - Rewind: Skip frames backward (using frame buffer)
- **Resolution Settings**: Configure camera resolution with dropdown selection
  - 640x480 (VGA)
  - 1280x720 (HD) 
  - 1920x1080 (Full HD)
- **Error Handling**: Comprehensive error handling with user notifications
- **Modern UI**: Clean, intuitive interface with grouped controls

## Requirements

### Dependencies

- **Qt6**: Core and Widgets modules
- **OpenCV**: Version 4.x recommended
- **CMake**: Version 3.16 or higher
- **C++17** compatible compiler

### Platform Support

- Windows 10/11
- Linux (Ubuntu 20.04+, other distributions)
- macOS (10.15+)

## Installation

### Windows

1. **Install Qt6**:
   - Download from [Qt.io](https://www.qt.io/download)
   - Or use vcpkg: `vcpkg install qt6-base qt6-widgets`

2. **Install OpenCV**:
   - Download from [OpenCV.org](https://opencv.org/releases/)
   - Or use vcpkg: `vcpkg install opencv[core,imgproc,highgui,videoio]`

3. **Install CMake**:
   - Download from [cmake.org](https://cmake.org/download/)

### Linux (Ubuntu/Debian)

```bash
# Install Qt6
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev cmake

# Install OpenCV
sudo apt install libopencv-dev

# Install build tools
sudo apt install build-essential
```

### macOS

```bash
# Using Homebrew
brew install qt6 opencv cmake

# Add Qt6 to PATH if needed
export CMAKE_PREFIX_PATH="/opt/homebrew/opt/qt6:$CMAKE_PREFIX_PATH"
```

## Building the Application

### Clone the Repository

```bash
git clone https://github.com/yourusername/qtbased.git
cd qtbased
```

### Build with CMake

```bash
# Create build directory
mkdir build
cd build

# Configure (adjust paths as needed)
cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt6

# Build
cmake --build . --config Release

# The executable will be in build/bin/
```

### Windows-specific Build

```bash
# Using Visual Studio
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release

# Using MinGW
cmake .. -G "MinGW Makefiles"
cmake --build .
```

## Running the Application

### From Build Directory

```bash
# Linux/macOS
./bin/QtCameraApp

# Windows
bin\QtCameraApp.exe
```

### Command Line Options

The application automatically detects the default camera (index 0). If you have multiple cameras, you may need to modify the camera index in the source code.

## Usage Guide

### Getting Started

1. **Launch the application**
2. **Camera initialization**: The app will automatically try to initialize your default camera
3. **Start recording**: Click the "Play" button to begin the camera feed

### Controls

- **Play**: Start the camera feed (disabled when running)
- **Pause**: Freeze the current frame (enabled when running)
- **Resume**: Continue from paused state (enabled when paused)
- **Forward**: Skip 10 frames forward
- **Rewind**: Go back 10 frames using the frame buffer
- **Resolution**: Select from dropdown to change camera resolution

### Resolution Settings

The application supports common resolutions:
- **640x480 (VGA)**: Standard definition, good performance
- **1280x720 (HD)**: High definition, balanced quality/performance
- **1920x1080 (Full HD)**: Maximum quality, requires good hardware

**Note**: Resolution changes require stopping and restarting the camera feed.

## Troubleshooting

### Common Issues

**Camera not detected**:
- Ensure your camera is not being used by another application
- Check camera permissions on your system
- Try different camera indices if you have multiple cameras

**Build errors**:
- Verify Qt6 and OpenCV are properly installed
- Check CMake can find the dependencies
- Ensure you're using a C++17 compatible compiler

**Performance issues**:
- Try lower resolution settings
- Close other applications using the camera
- Check system resources

### Error Messages

The application provides detailed error messages in dialog boxes and the status bar:
- Camera initialization failures
- Frame capture errors
- Resolution change issues

## Development

### Project Structure

```
qtbased/
├── CMakeLists.txt          # Main build configuration
├── README.md               # This file
├── .gitignore             # Git ignore rules
└── src/                   # Source code
    ├── main.cpp           # Application entry point
    ├── MainWindow.h/.cpp  # Main UI window
    └── CameraController.h/.cpp  # Camera management
```

### Code Architecture

- **MainWindow**: UI management and user interaction
- **CameraController**: OpenCV camera operations and frame management
- **Qt Timer**: Frame update mechanism (~30 FPS)
- **Error Handling**: Exception-based with user notifications

### Extending the Application

To add new features:
1. **New resolutions**: Add to `m_resolutions` vector in MainWindow constructor
2. **Camera settings**: Extend CameraController with new OpenCV properties
3. **Recording**: Add video writing capabilities using OpenCV VideoWriter
4. **Effects**: Implement image processing in the frame capture pipeline

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Qt Framework for the excellent UI toolkit
- OpenCV community for computer vision capabilities
- Contributors and testers

## Version History

- **v1.0.0**: Initial release with basic camera functionality
  - Live camera feed display
  - Playback controls (Play, Pause, Resume, Forward, Rewind)
  - Resolution configuration
  - Error handling and status reporting 