@echo off
cd ..
mkdir build 2>nul
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug
cd ..
cmake --build build
echo D | xcopy /s /y .\Sandbox\assets\ .\build\bin\assets
cd .\build\bin
.\Sandbox.exe
PAUSE
