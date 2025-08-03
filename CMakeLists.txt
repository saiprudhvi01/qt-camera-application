cmake_minimum_required(VERSION 3.16)
project(QtCameraApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set policy for Qt6
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

# Find required packages with better error handling
find_package(Qt6 COMPONENTS Core Widgets QUIET)
if(NOT Qt6_FOUND)
    message(FATAL_ERROR "Qt6 not found. Please install Qt6 or set CMAKE_PREFIX_PATH to Qt6 installation directory.")
endif()

find_package(OpenCV QUIET)
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found. Please install OpenCV or set OpenCV_DIR to OpenCV installation directory.")
endif()

# Qt6 specific settings
qt6_standard_project_setup()

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/CameraController.cpp
)

set(HEADERS
    src/MainWindow.h
    src/CameraController.h
)

# Create executable
qt6_add_executable(QtCameraApp ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(QtCameraApp
    Qt6::Core
    Qt6::Widgets
    ${OpenCV_LIBS}
)

# Include directories
target_include_directories(QtCameraApp PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    src
)

# Set output directory
set_target_properties(QtCameraApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Print configuration info
message(STATUS "Qt6 version: ${Qt6_VERSION}")
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV libs: ${OpenCV_LIBS}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Install target (optional)
install(TARGETS QtCameraApp
    RUNTIME DESTINATION bin
) 