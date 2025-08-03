@echo off
echo Starting Qt Camera Web Application...
echo.
echo This will start a local web server and open the camera application
echo in your default browser.
echo.
echo Requirements:
echo - Python 3.x installed
echo - Modern browser (Chrome, Firefox, Safari, Edge)
echo - Camera connected to your computer
echo.
pause

python server.py

if %errorlevel% neq 0 (
    echo.
    echo Error: Could not start the server.
    echo.
    echo Possible solutions:
    echo 1. Make sure Python 3 is installed and in your PATH
    echo 2. Try running: python3 server.py
    echo 3. Check if port 8080 is already in use
    echo.
    pause
) 