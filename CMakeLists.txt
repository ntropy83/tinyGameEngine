cmake_minimum_required(VERSION 3.11.0)

set(CMAKE_CROSSCOMPILING false)

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
if(WIN32)
    add_definitions(-DNOMINMAX)
endif()

# Include Toolchain for crosscompiling
if(CMAKE_CROSSCOMPILING)
    include("${CMAKE_SOURCE_DIR}/toolchain-mingw64.cmake")
    message(STATUS "Using toolchain-mingw64.cmake")

    # Include directories for Vulkan and Qt
    include_directories(${Vulkan_INCLUDE_DIRS})
    include_directories(${Qt5Core_INCLUDE_DIRS})
    include_directories(${Qt5Gui_INCLUDE_DIRS})
    include_directories(${Qt5Widgets_INCLUDE_DIRS})
else()
    find_package(Vulkan REQUIRED)
    find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
endif()

# Platform specific checks
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
    find_package(X11 REQUIRED)
elseif(APPLE)
    return()  # Exit the CMakeLists.txt for macOS
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

qt_add_executable(${NAME} ${PROJECT_SOURCES})

# Qt version-specific setup and linking
if(NOT CMAKE_CROSSCOMPILING)
    # Link to the appropriate Qt modules
    target_link_libraries(${NAME} PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core Vulkan::Vulkan glfw physfs glslang glslang-default-resource-limits SPIRV MachineIndependent GenericCodeGen OSDependent)
else()
    # When cross-compiling, link directly to the Qt SDK libraries
    target_link_libraries(${NAME} PRIVATE ${Vulkan_LIBRARY} glfw physfs glslang glslang-default-resource-limits SPIRV MachineIndependent GenericCodeGen OSDependent)
endif()

# Set target properties
set_target_properties(${NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# For Linux, link the X11 libraries
if(LINUX)
    target_link_libraries(${NAME} PRIVATE ${X11_LIBRARIES})
    target_include_directories(${NAME} PRIVATE ${X11_INCLUDE_DIR})
endif()

# Additional target properties for cross-compiling
if(CMAKE_CROSSCOMPILING)
    # Set properties specific to Windows when cross-compiling
    set_target_properties(${NAME} PROPERTIES SUFFIX ".exe")
endif()
