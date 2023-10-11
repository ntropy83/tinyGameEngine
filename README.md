# tiny GameEngine in Vulkan
Playground to create a tiny vulkan game engine

Implemented:

Vulkan Pipeline - command and frame buffer
PhysFS          - an isolated file tree from a zip for assets
simdJson        - read Json config files quickly(integrated but not implemented yet)
glslang         - runtime shader compiler from GLSL to SPIR-V

Release - Milestone 1: red vulkan triangle

# Build

```
git clone --recursive https://github.com/ntropy83/tinyGameEngine
mkdir build && cd build
cmake .. && make -j${nproc}
```
All necessary runtime files go to build/debug.

![Vulkan Triangle](https://github.com/ntropy83/tinyGameEngine/blob/main/screenshot_triangle.png?raw=true)