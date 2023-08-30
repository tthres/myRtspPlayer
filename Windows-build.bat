@echo off
if not exist "build\" (
    echo "Directory build not found. Creating it..."
    mkdir build
    if errorlevel 1 (
        echo "Failed to create build directory."
        exit /b
    )
)

cd build
if errorlevel 1 (
    echo "Directory build not found."
    exit /b
)

echo "Removing all files in build directory..."
del /s /q *
if errorlevel 1 (
    echo "Failed to delete files in build directory."
    exit /b
)

echo "Running CMake..."
cmake .. -A Win32
if errorlevel 1 (
    echo "CMake configuration failed."
    exit /b
)

echo "Building with CMake..."
cmake --build .
if errorlevel 1 (
    echo "CMake build failed."
    pause
    exit /b
)

echo "Build successful."
pause
