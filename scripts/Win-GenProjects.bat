@echo off
setlocal
set "ROOT_DIR=%~dp0.."
cd /d "%ROOT_DIR%"
if not exist build mkdir build
cd /d "%ROOT_DIR%\build"
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug
if errorlevel 1 (
    echo CMake generation failed!
    pause
    exit /b 1
)
cd /d "%ROOT_DIR%"
cmake --build build
if errorlevel 1 (
    echo CMake build failed!
    pause
    exit /b 1
)
echo D | xcopy /s /y "%ROOT_DIR%\Sandbox\assets" "%ROOT_DIR%\build\bin\assets"
cd /d "%ROOT_DIR%\build\bin"
MochiiEditor.exe
