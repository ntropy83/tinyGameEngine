cmake_minimum_required(VERSION 2.8)
CMAKE_POLICY ( SET CMP0003 NEW )

set(CMAKE_BUILD_TYPE MINSIZEREL)

project(tinyGameEngine CXX)

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -O2")
SET(GCC_COVERAGE_LINK_FLAGS    "-ldl -lX11 -lXxf86vm -lXrandr -lXi")

find_package(Vulkan REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/engine/include)
include_directories(${CMAKE_SOURCE_DIR}/editor/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/submodules)
include_directories(${CMAKE_SOURCE_DIR}/editor/submodules)
include_directories(${CMAKE_SOURCE_DIR}/editor/submodules/imgui)

include_directories(${CMAKE_SOURCE_DIR}/editor/assets)

# options GLFW
set(GLFW_LIBRARY_TYPE "SHARED")
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)

# options GLSLANG
option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
set(SKIP_GLSLANG_INSTALL ON)
option(ENABLE_SPVREMAPPER "Enables building of SPVRemapper" OFF)
option(ENABLE_GLSLANG_BINARIES "Builds glslang and spirv-remap" OFF)
option(ENABLE_OPT "Enables spirv-opt capability if present" OFF)
option(ENABLE_HLSL "Enables HLSL input support" OFF)

# options PHYSFS
option(PHYSFS_BUILD_STATIC "Build static library" FALSE)
option(PHYSFS_BUILD_TEST "Build stdio test program." FALSE)
option(PHYSFS_BUILD_DOCS "Build doxygen based documentation" FALSE)
set(PHYSFS_TARGETNAME_UNINSTALL "uninstall_1" CACHE STRING "Name of 'uninstall' build target")

# options simdjson
option(SIMDJSON_BUILD_STATIC_LIB "Build simdjson_static library along with simdjson" OFF)
option(BUILD_SHARED_LIBS "Build simdjson as a shared library" ON)

# lpthread fix
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

## Dear ImGui
set(IMGUI_PATH  "engine/submodules/imgui")

file(GLOB IMGUI_SOURCES
        "editor/submodules/imgui/*.h"
        "editor/submodules/imgui/*.cpp")
add_library(imgui SHARED ${IMGUI_SOURCES})

## Paths and package of game assets

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/debug/lib)

add_custom_target(build-time-make-directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data)

function(create_zip output_file input_files working_dir)
    add_custom_command(
        COMMAND ${CMAKE_COMMAND} -E tar "cf" "${output_file}" --format=zip -- ${input_files}
        WORKING_DIRECTORY "${working_dir}"
        OUTPUT  "${output_file}"
        DEPENDS ${input_files}
        COMMENT "Zipping to ${output_file}."
    )
endfunction()

file(GLOB ZIP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/editor/assets/*")
create_zip("${CMAKE_CURRENT_SOURCE_DIR}/build/debug/data/data.zip" "${ZIP_FILES}" "${CMAKE_CURRENT_SOURCE_DIR}/editor/assets")

add_custom_target("project-data" ALL DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/build/debug/data/data.zip")

## Include subdirectories

add_subdirectory("engine/submodules/glfw")
add_subdirectory("engine/submodules/glslang")
add_subdirectory("engine/submodules/physfs")
add_subdirectory("engine/submodules/simdjson")

add_executable(tinyGameEngine
                ## Vulkan Engine
                "engine/src/file/filesystem.cpp"
                "engine/src/file/JsonFile.cpp"
                "engine/src/vulkan/tge_shadercomp.cpp"
                "engine/src/vulkan/tge_device.cpp"
                "engine/src/vulkan/tge_pipeline.cpp"
                "engine/src/vulkan/tge_swap_chain.cpp"
                "engine/src/vulkan/tge_window.cpp"
                # Editor                
                "editor/src/JsonFiles.cpp"
                "editor/src/main.cpp"
                "editor/src/tge_editor.cpp"
                "editor/src/ui/tge_gui.cpp"
                ## ImGUI
                "editor/submodules/imgui/imgui.cpp"
                "editor/submodules/imgui/imgui_demo.cpp"
                "editor/submodules/imgui/imgui_draw.cpp"
                "editor/submodules/imgui/imgui_tables.cpp"
                "editor/submodules/imgui/imgui_widgets.cpp"
                "editor/submodules/imgui/backends/imgui_impl_vulkan.cpp"
                "editor/submodules/imgui/backends/imgui_impl_glfw.cpp")

target_link_libraries(tinyGameEngine 
                        Vulkan::Vulkan 
                        glfw 
                        simdjson physfs
                        glslang glslang-default-resource-limits SPIRV MachineIndependent GenericCodeGen OSDependent
                        imgui)