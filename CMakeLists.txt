cmake_minimum_required(VERSION 3.11.0)

message(STATUS "Using ${CMAKE_GENERATOR}")

set(NAME tgEngine)
project(${NAME} VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Check for cross-compiling and set up the MinGW toolchain
if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling with MinGW")
    set(CMAKE_SYSTEM_NAME Windows)
    set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)

    # set compilers
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
    set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)
    
    # set sysroot
    set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})
    
    # adjust default behavior of FIND_XXX() commands
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    
    # Set the WIN32 flag to true to ensure Windows code paths are taken
    add_definitions(-DWIN32)
endif()

include_directories(${CMAKE_SOURCE_DIR}/editor/src)
include_directories(${CMAKE_SOURCE_DIR}/editor/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/submodules)

# Define NOMINMAX to avoid issues with std::min/max and the Windows macro
add_definitions(-DNOMINMAX)

# Vulkan setup
find_package(Vulkan REQUIRED)
message(STATUS "Found Vulkan: ${Vulkan_LIBRARIES}")

# Platform specific checks
if(UNIX AND NOT APPLE AND NOT CMAKE_CROSSCOMPILING)
    set(LINUX TRUE)
    find_package(X11 REQUIRED)
elseif(APPLE)
    return()  # Exit the CMakeLists.txt for macOS
endif()

# Qt setup
find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Widgets LinguistTools Gui Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Gui Core)

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

# Qt version-specific setup
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(${NAME} SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(${NAME}
            ${PROJECT_SOURCES}
        )
    endif()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# Linking libraries
target_link_libraries(${NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Core Vulkan::Vulkan glfw physfs glslang glslang-default-resource-limits SPIRV MachineIndependent GenericCodeGen OSDependent)

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
