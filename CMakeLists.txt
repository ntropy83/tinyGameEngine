cmake_minimum_required(VERSION 3.11.0)

set(CMAKE_CROSSCOMPILING false)

# Determine if the MinGW compiler is being used
if(CMAKE_CROSSCOMPILING)
    set(CMAKE_TOOLCHAIN_FILE "toolchain-mingw64.cmake" CACHE FILEPATH "toolchain-mingw64.cmake")
    message(STATUS "Using toolchain-mingw64.cmake")
endif()

message(STATUS "Using ${CMAKE_GENERATOR}")

set(NAME tgEngine)
project(${NAME} VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(${CMAKE_SOURCE_DIR}/editor/src)
include_directories(${CMAKE_SOURCE_DIR}/editor/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/submodules)

# Define NOMINMAX to avoid issues with std::min/max and the Windows macro
add_definitions(-DNOMINMAX)

# Vulkan setup
if(NOT CMAKE_CROSSCOMPILING)
    find_package(Vulkan REQUIRED)
    message(STATUS "Found Vulkan: ${Vulkan_LIBRARIES}")
else()
    # Assume Vulkan paths are set in the toolchain file
    set(Vulkan_INCLUDE_DIRS ${Vulkan_INCLUDE_DIRS} CACHE PATH "Vulkan include directory")
    set(Vulkan_LIBRARY ${Vulkan_LIBRARY} CACHE FILEPATH "Vulkan library file")
    message(STATUS "Using Vulkan from the toolchain file")
endif()

# Platform specific checks
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
    find_package(X11 REQUIRED)
elseif(APPLE)
    return()  # Exit the CMakeLists.txt for macOS
endif()

# Qt setup
if(CMAKE_CROSSCOMPILING)
    # When cross-compiling, assume Qt paths are set in the toolchain file
    include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
    link_directories(${Qt5Core_LIBRARY} ${Qt5Gui_LIBRARY} ${Qt5Widgets_LIBRARY})
    message(STATUS "Using Qt from the toolchain file")
else()
    # Use the standard find_package call for native builds
    find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Widgets LinguistTools Gui Core)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Gui Core)
endif()


set(TS_FILES editor/lang/tge_qt_gui_de_DE.ts)

set(PROJECT_SOURCES
    editor/src/debug/tge_QtDebug.cpp
    editor/src/main.cpp
    editor/include/ui/tge_mainwindow.h
    editor/src/ui/tge_mainwindow.cpp
    editor/include/ui/tge_splashscreen.h
    editor/src/ui/tge_splashscreen.cpp
    editor/src/ui/qt_ui/mainwindow.ui
    editor/src/ui/tge_vulkanwindow.cpp
    engine/src/debug/tge_vulDebug.cpp
    engine/src/file/filesystem.cpp
    engine/src/vulkan/tge_shadercomp.cpp
    engine/src/vulkan/tge_device.cpp
    engine/src/vulkan/tge_pipeline.cpp
    engine/src/vulkan/tge_swap_chain.cpp
    engine/include/vulkan/tge_window.hpp
    engine/src/vulkan/tge_window.cpp
)

add_subdirectory(engine)

qt_add_executable(${NAME}
${PROJECT_SOURCES}
)

# Qt version-specific setup and linking
if(NOT CMAKE_CROSSCOMPILING)
    # Link to the appropriate Qt modules
    target_link_libraries(${NAME} PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core Vulkan::Vulkan glfw physfs glslang glslang-default-resource-limits SPIRV MachineIndependent GenericCodeGen OSDependent)
else()
    # When cross-compiling, link directly to the Qt SDK libraries
    target_link_libraries(${NAME} PRIVATE Qt5Widgets Qt5Gui Qt5Core ${Vulkan_LIBRARY} glfw physfs glslang glslang-default-resource-limits SPIRV MachineIndependent GenericCodeGen OSDependent)
endif()

# Set target properties
set_target_properties(${NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    WIN32_EXECUTABLE TRUE
)

# For Linux, link the X11 libraries
if(LINUX)
    target_link_libraries(${NAME} PRIVATE ${X11_LIBRARIES})
    target_include_directories(${NAME} PRIVATE ${X11_INCLUDE_DIR})
endif()

# Additional target properties for cross-compiling
if(CMAKE_CROSSCOMPILING)
    # Here you would set properties specific to Windows when cross-compiling,
    # such as .exe suffix for binaries, or any Windows-specific compile options
    set_target_properties(${NAME} PROPERTIES SUFFIX ".exe")
endif()
