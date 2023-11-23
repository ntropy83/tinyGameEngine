# tiny GameEngine in Vulkan
Playground to create a tiny vulkan game engine and editor

Implemented:

Vulkan Pipeline - graphics pipeline and Simple Render system

PhysFS          - an isolated file tree from a zip for assets

glslang         - runtime shader compiler from GLSL to SPIR-V

imGUI           - implemented

glm             - math library

stb             - image loader library

Qt6             - create a Window Editor

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
The respective paths can be set within the toolchain-mingw64.cmake. For the find_package command to work with Vulkan, you may have to change the Include dir of the SDK to lowercase. Set the crosscompile variable in the main CMakelists.txt to true.

![Vulkan Triangle](https://github.com/ntropy83/tinyGameEngine/blob/main/screenshot_triangle_qt.png?raw=true)

Based on the Vulkan Tutorial: https://www.youtube.com/watch?v=Y9U9IE0gVHA&list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR&index=1&t=0s

# To be implemented
- 3D Model System and loading
    - use Quaternions for rotation
- Entity Component System for GameObjects
- Material System
- Lightning System

# file structure 
Two CMakeLists.txt, one on the root level, one in the directory engine to configure the submodules. 
The Vulkan Pipeline is configured in engine/src/vulkan and finishes in in a Render System in engine/src/vulkan/systems.
In editor the main.cpp runs the App, implementing a GLFW window in a MainWindow that gets refreshed with 60 fps (17 msecs). 
The tge_vulkanwindow provides the GLFW window and Vulkan Pipeline, while tge_mainwindow handles Qt.
The assets folder gets compressed to a zip and read in by the application in a virtual filesystem via PhysFS.
The assets folder contains the GLSL shaders, which get compiled at runtime via glslang.
Another submodule is the math library glm and the overlay GUI imGUI, which is not implementd yet. 

# filetree
```
├── editor
│   ├── assets
│   │   ├── game.json
│   │   ├── pipeline
│   │   │   └── shaders.json
│   │   ├── shaders
│   │   │   ├── simple_shader.frag
│   │   │   └── simple_shader.vert
│   │   └── shaders_includes
│   │       ├── uniform_renderer.glsl
│   │       └── utils.glsl
│   ├── include
│   │   ├── debug
│   │   │   └── tge_QtDebug.hpp
│   │   ├── tge_game_object.hpp
│   │   └── ui
│   │       ├── tge_mainwindow.h
│   │       ├── tge_splashscreen.h
│   │       └── tge_vulkanwindow.hpp
│   └── src
│       ├── debug
│       │   └── tge_QtDebug.cpp
│       ├── main.cpp
│       └── ui
│           ├── qt_ui
│           │   ├── mainwindow.ui
│           │   ├── tge_settingsdialog.ui
│           │   └── tge_splashscreen.ui
│           ├── tge_mainwindow.cpp
│           ├── tge_splashscreen.cpp
│           └── tge_vulkanwindow.cpp
├── engine
│   ├── CMakeLists.txt
│   ├── include
│   │   ├── debug
│   │   │   └── tge_vulDebug.hpp
│   │   ├── file
│   │   │   └── filesystem.hpp
│   │   └── vulkan
│   │       ├── systems
│   │       │   └── simple_render_system.hpp
│   │       ├── tge_device.hpp
│   │       ├── tge_model.hpp
│   │       ├── tge_pipeline.hpp
│   │       ├── tge_renderer.hpp
│   │       ├── tge_shadercomp.hpp
│   │       ├── tge_swap_chain.hpp
│   │       └── tge_window.hpp
│   ├── src
│   │   ├── debug
│   │   │   └── tge_vulDebug.cpp
│   │   ├── file
│   │   │   └── filesystem.cpp
│   │   └── vulkan
│   │       ├── systems
│   │       │   └── simple_render_system.cpp
│   │       ├── tge_device.cpp
│   │       ├── tge_model.cpp
│   │       ├── tge_pipeline.cpp
│   │       ├── tge_renderer.cpp
│   │       ├── tge_shadercomp.cpp
│   │       ├── tge_swap_chain.cpp
│   │       └── tge_window.cpp
│   └── submodules
│       ├── glfw
│       ├── glm
│       ├── glslang
│       ├── imgui
│       ├── physfs
│       └── stb
```