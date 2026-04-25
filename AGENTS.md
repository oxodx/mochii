# AGENTS.md - Guidelines for AI Coding Agents

- Make minimal changes; avoid drive-by refactors.
- Do not commit secrets.

## Build Commands

Debug: `cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug && cmake --build build`
Release: `cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build build`

Or use `scripts\build-debug.bat` / `scripts\build-release.bat`.

## Asset Setup

After build, copy `Sandbox/assets` and `MochiiEditor/assets` to `build/bin/assets`.

## Structure

- `Mochii/` - core static library
- `MochiiEditor/` - editor app (builds to `build/bin/MochiiEditor.exe`)
- `Sandbox/` - test app

Executables: `build/bin/`.

## Formatting

- cpp/h: `clang-format` (style: Google, see `.clang-format`)

## References

- Readme: `README.md`
- CI: `.github/workflows/build.yml`
- Root CMake: `CMakeLists.txt`
