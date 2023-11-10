# tiny GameEngine in Vulkan
Playground to create a tiny vulkan game engine and editor

Implemented:

Vulkan Pipeline - command and frame buffer

PhysFS          - an isolated file tree from a zip for assets

glslang         - runtime shader compiler from GLSL to SPIR-V

Qt6             - create an Editor

# Build
Deps on Ubuntu:
```
sudo apt-get install vulkan-validationlayers-dev \
                     spirv-tools libxxf86vm-dev libxi-dev libxrandr-dev \
                     libxinerama-dev libxcursor-dev mesa-common-dev qt5-default libvulkan-dev
```

```
git clone --recursive https://github.com/ntropy83/tinyGameEngine
mkdir build && cd build
cmake .. && make -j${nproc}
```
# Crosscompile
(WIP) To crosscompile from Linux to Windows, create a folder crosscompile in the project root and copy the Vulkan SDK and Qt6 SDK.
The respective paths can be set within the toolchain-mingw64.cmake.

![Vulkan Triangle](https://github.com/ntropy83/tinyGameEngine/blob/main/screenshot_triangle_qt.png?raw=true)