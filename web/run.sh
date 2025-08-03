#!/bin/bash

echo "Starting Qt Camera Web Application..."
echo
echo "This will start a local web server and open the camera application"
echo "in your default browser."
echo
echo "Requirements:"
echo "- Python 3.x installed"
echo "- Modern browser (Chrome, Firefox, Safari)"
echo "- Camera connected to your computer"
echo

# Check if Python 3 is available
if command -v python3 &> /dev/null; then
    PYTHON_CMD="python3"
elif command -v python &> /dev/null; then
    PYTHON_CMD="python"
else
    echo "Error: Python 3 is not installed or not in PATH"
    echo "Please install Python 3 and try again"
    exit 1
fi

echo "Using Python command: $PYTHON_CMD"
echo
read -p "Press Enter to continue..."

$PYTHON_CMD server.py

if [ $? -ne 0 ]; then
    echo
    echo "Error: Could not start the server."
    echo
    echo "Possible solutions:"
    echo "1. Make sure Python 3 is installed"
    echo "2. Check if port 8080 is already in use"
    echo "3. Try running with sudo if there are permission issues"
    echo
fi 