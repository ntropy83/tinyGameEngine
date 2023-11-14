# tiny GameEngine in Vulkan
Playground to create a tiny vulkan game engine and editor

Implemented:

Vulkan Pipeline - command and frame buffer

PhysFS          - an isolated file tree from a zip for assets

glslang         - runtime shader compiler from GLSL to SPIR-V

Qt6             - create an Editor

# Build
Deps on Arch Linux:
```
sudo pacman -S vulkan-devel qt6-base qt6-declarative
```

Deps on Windows:
```
Qt6 SDK, Vulkan SDK 
```

```
git clone --recursive https://github.com/ntropy83/tinyGameEngine
mkdir build && cd build
cmake .. && make -j${nproc}
```
# Crosscompile
To crosscompile from Linux to Windows, create a folder in the project root and copy the Vulkan SDK and Qt6 SDK for Windows.
The respective paths can be set within the toolchain-mingw64.cmake. For the find_package command to work with Vulkan, you may have to change the Include dir of the SDK to lowercase.

![Vulkan Triangle](https://github.com/ntropy83/tinyGameEngine/blob/main/screenshot_triangle_qt.png?raw=true)

Based on the Vulkan Tutorial: https://www.youtube.com/watch?v=Y9U9IE0gVHA&list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR&index=1&t=0s