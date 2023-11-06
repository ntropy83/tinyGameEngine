cmake_minimum_required(VERSION 3.11.0)

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

# Vulkan setup
find_package(Vulkan REQUIRED)
message(STATUS "Found Vulkan: ${Vulkan_LIBRARIES}")

# Platform specific checks
if(UNIX AND NOT APPLE)
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
if(LINUX)
    target_link_libraries(${NAME} PRIVATE ${X11_LIBRARIES})
    target_include_directories(${NAME} PRIVATE ${X11_INCLUDE_DIR})
elseif(APPLE)
    # Link any macOS-specific frameworks here if necessary
endif()

target_link_libraries(${NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Core Vulkan::Vulkan glfw physfs glslang glslang-default-resource-limits SPIRV MachineIndependent GenericCodeGen OSDependent)

# Set target properties
if(NOT APPLE)
    set_target_properties(${NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
    )
endif()
