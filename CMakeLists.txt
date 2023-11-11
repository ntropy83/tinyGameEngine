cmake_minimum_required(VERSION 3.19)
cmake_policy(SET CMP0144 OLD)

set(CMAKE_CROSSCOMPILING true)

set(NAME tgEngine)

if(CMAKE_CROSSCOMPILING)
    include("${CMAKE_SOURCE_DIR}/toolchain-mingw64.cmake")
    message(STATUS "Using toolchain-mingw64.cmake")
endif()

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
# Avoid RPATH changes on install with Ninja generator
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

# Find packages
find_package(Vulkan REQUIRED)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Include Vulkan and Qt
include_directories(${Vulkan_INCLUDE_DIRS})
include_directories(${Qt6Core_INCLUDE_DIRS})
include_directories(${Qt6Gui_INCLUDE_DIRS})
include_directories(${Qt6Widgets_INCLUDE_DIRS})

# Platform specific checks
if(UNIX AND NOT APPLE AND NOT WIN32)
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

# Define the executable
add_executable(${NAME} ${PROJECT_SOURCES})
target_link_libraries(${NAME} PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core Vulkan::Vulkan glfw physfs glslang glslang-default-resource-limits SPIRV)

# Set target properties
set_target_properties(${NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# For Linux, link the X11 libraries
if(LINUX)
    target_link_libraries(${NAME} PRIVATE ${X11_LIBRARIES})
    target_include_directories(${NAME} PRIVATE ${X11_INCLUDE_DIR})
endif()

# Copy libraries next to the executable
add_custom_command(TARGET ${NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:glfw> $<TARGET_FILE_DIR:${NAME}>
COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:glslang> $<TARGET_FILE_DIR:${NAME}>
COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:physfs> $<TARGET_FILE_DIR:${NAME}>
COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:glslang-default-resource-limits> $<TARGET_FILE_DIR:${NAME}>
COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SPIRV> $<TARGET_FILE_DIR:${NAME}>                
# Repeat for other submodule libraries
)

# Additional target properties for cross-compiling
if(CMAKE_CROSSCOMPILING)
    # Set properties specific to Windows when cross-compiling
    set_target_properties(${NAME} PROPERTIES SUFFIX ".exe")

    add_custom_command(TARGET ${NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${QT_DLL_DIR}/Qt6Core.dll"
        "${QT_DLL_DIR}/Qt6Gui.dll"
        "${QT_DLL_DIR}/Qt6Widgets.dll"
        "${MINGW_DLL_DIR}/libgcc_s_seh-1.dll"
        "${MINGW_DLL_DIR}/libwinpthread-1.dll"
        "${MINGW_DLL_DIR}/libstdc++-6.dll"
        # Add other necessary DLLs here
        $<TARGET_FILE_DIR:${NAME}>)

endif()
