# imgui-sdl2-opengl3-starter

## How to configure with CMake

### Linux
From bash terminal:
```
cmake --preset ninjam-linux
```

### Windows
Available presets:

- `vs2022`

  From powershell terminal:
  ```powershell
  cmake --preset vs2022
  ```
  You should use VS Community 2022 and this preset on Windows.

- `vs2022-32`

  From powershell terminal:
  ```powershell
  cmake --preset vs2022-32
  ```

- `ninjam-msvc`

  From powershell terminal:
  ```powershell
  cmd
  "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
  cmake --preset ninjam-msvc
  ```
  Please correct the path to `vcvars64.bat` or `vcvars32.bat`

- `ninjam-msys2`

  You need to patch SDL2 port first, then you can configure it. This bug is [fixed](https://github.com/libsdl-org/SDL/commit/d2c4d74dd0302369251dce9375b49f394e54f76a) but is not released yet. You also need to configure in MSYS2 MinGW terminal to make sure you use the correct MinGW's `cmake`

  From MSYS2 MinGW 64-bit terminal run these commands:
  ```
  cp patch/sdl2/mingw-windows-devices.patch vcpkg/ports/sdl2/mingw-windows-devices.patch
  sed -i '/0004-Define-crt-macros.patch/a \ \ \ \ \ \ \ \ mingw-windows-devices.patch' vcpkg/ports/sdl2/portfile.cmake
  cmake --preset ninjam-msys2
  ```


## How to debug with VS Code

You only need Microsoft C/C++ extension (cpptools).

- Intenllisense profiles are stored in `.vscode/c_cpp_properties.json`
- Build commands are store in `.vscode/tasks.json`
- Debug profiles are stored in `.vscode/launch.json`
- Open `.vscode/imgui-sdl2-opengl3-starter.code-workspace` with VS Code

### Linux

- For `ninjam-linux`: it works out of the box, nothing to do.

### Windows

- For `ninjam-msys2`: you need to set `MINGW_ROOT` environment var to `/mingw64` folder (or `/mingw32`, etc.). For example `D:\msys64\mingw64`

- For `ninjam-msvc`: you need to open VS Code with `vcvars64.bat` (or `vcvars32.bat`) enabled. For example from powershell terminal run:

  ```
  cmd
  "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
  code .vscode/imgui-sdl2-opengl3-starter.code-workspace
  ```

## How to use external libraries

- Add the library's names to `vcpkg.json` `"dependencies": [ ... ]"`
- Run `cmake --preset ...` again. `vcpkg` will build and installed those libraries for you. In addition to that, `vcpkg` will tell you how to find and link those libraries with CMake.
- Add `find_package(...)` and `target_link_libraries(...)` to `CMakeList.txt`
