@echo off
echo Building Qt Camera Application for Windows...

REM Create build directory
if not exist build mkdir build
cd build

REM Try to configure with common Qt6 paths
echo Attempting to configure with CMake...

REM Option 1: Try with Qt6 in PATH
cmake .. -G "MinGW Makefiles" 2>nul
if %errorlevel% equ 0 goto build

REM Option 2: Try with common Qt6 installation paths
echo Trying common Qt6 installation paths...
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\Qt\6.7.0\mingw_64" 2>nul
if %errorlevel% equ 0 goto build

cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\Qt\6.6.0\mingw_64" 2>nul
if %errorlevel% equ 0 goto build

cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\Qt\6.5.0\mingw_64" 2>nul
if %errorlevel% equ 0 goto build

REM Option 3: Try with vcpkg
echo Trying vcpkg toolchain...
cmake .. -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake" 2>nul
if %errorlevel% equ 0 goto build

echo.
echo ERROR: CMake configuration failed!
echo.
echo Please ensure that:
echo 1. Qt6 is installed and CMAKE_PREFIX_PATH is set correctly
echo 2. OpenCV is installed and can be found by CMake
echo 3. MinGW or Visual Studio is available
echo.
echo Example usage:
echo   set CMAKE_PREFIX_PATH=C:\Qt\6.7.0\mingw_64
echo   %~nx0
echo.
echo Or install dependencies with vcpkg:
echo   vcpkg install qt6-base qt6-widgets opencv
echo.
pause
exit /b 1

:build
echo Configuration successful! Building...
cmake --build . --config Release

if %errorlevel% equ 0 (
    echo.
    echo Build successful! 
    echo Executable location: build\bin\QtCameraApp.exe
    echo.
    echo To run the application:
    echo   cd build\bin
    echo   QtCameraApp.exe
    echo.
) else (
    echo.
    echo Build failed! Check the error messages above.
    echo.
)

pause 