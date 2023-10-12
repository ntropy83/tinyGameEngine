# tiny GameEngine in Vulkan
Playground to create a tiny vulkan game engine and editor

Implemented:

Vulkan Pipeline - command and frame buffer

PhysFS          - an isolated file tree from a zip for assets

simdJson        - read Json config files quickly(integrated but not implemented yet)

glslang         - runtime shader compiler from GLSL to SPIR-V

Dear ImGUI      - glfw and Vulkan compatible GUI

# Build
Deps on Ubuntu:
```
sudo apt-get install vulkan-validationlayers-dev \
                     spirv-tools libxxf86vm-dev libxi-dev libxrandr-dev \
                     libxinerama-dev libxcursor-dev mesa-common-dev
```

```
git clone --recursive https://github.com/ntropy83/tinyGameEngine
mkdir build && cd build
cmake .. && make -j${nproc}
```
All necessary runtime files go to build/debug.

![Vulkan Triangle](https://github.com/ntropy83/tinyGameEngine/blob/main/screenshot_triangle.png?raw=true)