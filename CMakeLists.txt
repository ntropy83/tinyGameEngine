cmake_minimum_required(VERSION 3.19)
cmake_policy(SET CMP0144 OLD)

# Enable crosscompiling
set(CMAKE_CROSSCOMPILING false)

set(NAME tgEngine)

if(CMAKE_CROSSCOMPILING)
    include("${CMAKE_SOURCE_DIR}/toolchain-mingw64.cmake")
    message(STATUS "Using toolchain-mingw64.cmake")
endif()

project(${NAME} VERSION 0.2.0 LANGUAGES CXX)

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

# Define directories for binaries and libraries
set(BIN_DIR "${CMAKE_BINARY_DIR}")
set(LIB_DIR "${BIN_DIR}/libs")

add_subdirectory(engine)

# Define the executable
add_executable(${NAME} ${PROJECT_SOURCES})
target_link_libraries(${NAME} PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core Vulkan::Vulkan glfw physfs-static glslang glslang-default-resource-limits SPIRV)

# Set target properties
set_target_properties(${NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# For Linux, link the X11 libraries
if(LINUX)
    target_link_libraries(${NAME} PRIVATE ${X11_LIBRARIES})
    target_include_directories(${NAME} PRIVATE ${X11_INCLUDE_DIR})
endif()

# Copy executable to 'bin' directory and libraries to 'bin/libs'
add_custom_command(TARGET ${NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${BIN_DIR}
#    COMMAND ${CMAKE_COMMAND} -E make_directory ${LIB_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${NAME}> ${BIN_DIR}
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:glfw> ${LIB_DIR}
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:glslang> ${LIB_DIR}
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:physfs-static> ${LIB_DIR}
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:glslang-default-resource-limits> ${LIB_DIR}
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SPIRV> ${LIB_DIR}
)

# Copy Resource files
# Assuming the source file is in the 'ico' directory relative to your project root
set(SOURCE_FILE "${CMAKE_SOURCE_DIR}/ico/splash.png")

# Set the destination directory and file
set(DEST_DIR "${BIN_DIR}/res")
set(DEST_FILE "${DEST_DIR}/splash.png")

# Ensure the destination directory exists
add_custom_command(
    OUTPUT ${DEST_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_DIR}
)

# Copy the file
add_custom_command(
    OUTPUT ${DEST_FILE}
    COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_FILE} ${DEST_FILE}
    DEPENDS ${SOURCE_FILE}
)

# Add a custom target that depends on the copied file
add_custom_target(CopySplash ALL
    DEPENDS ${DEST_FILE}
)

# Additional target properties for cross-compiling
if(CMAKE_CROSSCOMPILING)
    # Set properties specific to Windows when cross-compiling
    set_target_properties(${NAME} PROPERTIES SUFFIX ".exe")

    # Copy Qt and MinGW DLLs to 'bin' directory
    add_custom_command(TARGET ${NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_DLL_DIR}/Qt6Core.dll" ${BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_DLL_DIR}/Qt6Gui.dll" ${BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_DLL_DIR}/Qt6Widgets.dll" ${BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MINGW_DLL_DIR}/libgcc_s_seh-1.dll" ${BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MINGW_DLL_DIR}/libwinpthread-1.dll" ${BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MINGW_DLL_DIR}/libstdc++-6.dll" ${BIN_DIR}
    )

    # Add custom command to execute windeployqt6.exe using Wine
    add_custom_command(TARGET tgEngine POST_BUILD
        COMMAND wine ${CMAKE_SOURCE_DIR}/crosscompile/qt6/6.6.0/mingw_64/bin/windeployqt6.exe ${BIN_DIR}/tgEngine.exe
        WORKING_DIRECTORY $<TARGET_FILE_DIR:tgEngine>
        COMMENT "Running windeployqt6.exe using Wine"
    )

endif()
