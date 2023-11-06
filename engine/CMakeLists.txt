cmake_minimum_required(VERSION 3.7)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
set(CMAKE_BUILD_TYPE DEBUG)

include_directories(${CMAKE_SOURCE_DIR}/editor/assets)
include_directories(${CMAKE_SOURCE_DIR}/engine/submodules/glfw/include)

# options GLFW
set(GLFW_LIBRARY_TYPE "SHARED")
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)

# options GLSLANG
option(BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
set(SKIP_GLSLANG_INSTALL OFF)
option(ENABLE_SPVREMAPPER "Enables building of SPVRemapper" OFF)
option(ENABLE_GLSLANG_BINARIES "Builds glslang and spirv-remap" OFF)
option(ENABLE_OPT "Enables spirv-opt capability if present" OFF)
option(ENABLE_HLSL "Enables HLSL input support" OFF)

# options PHYSFS
option(PHYSFS_BUILD_STATIC "Build static library" FALSE)
option(PHYSFS_BUILD_TEST "Build stdio test program." FALSE)
option(PHYSFS_BUILD_DOCS "Build doxygen based documentation" FALSE)
set(PHYSFS_TARGETNAME_UNINSTALL "uninstall_1" CACHE STRING "Name of 'uninstall' build target")
option(PHYSFS_ARCHIVE_7Z "Enable 7zip support" FALSE)
option(PHYSFS_ARCHIVE_GRP "Enable Build Engine GRP support" FALSE)
option(PHYSFS_ARCHIVE_WAD "Enable Doom WAD support" FALSE)
option(PHYSFS_ARCHIVE_CSM "Enable Chasm: The Rift CSM.BIN support" FALSE)
option(PHYSFS_ARCHIVE_HOG "Enable Descent I/II HOG support" FALSE)
option(PHYSFS_ARCHIVE_MVL "Enable Descent I/II MVL support" FALSE)
option(PHYSFS_ARCHIVE_QPAK "Enable Quake I/II QPAK support" FALSE)
option(PHYSFS_ARCHIVE_SLB "Enable I-War / Independence War SLB support" FALSE)
option(PHYSFS_ARCHIVE_ISO9660 "Enable ISO9660 support" FALSE)
option(PHYSFS_ARCHIVE_VDF "Enable Gothic I/II VDF archive support" FALSE)

## Paths and package of game assets

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

# Specify a relative path for the zip file
set(ZIP_FILE "${CMAKE_BINARY_DIR}/data.zip")

function(create_zip output_file input_files working_dir)
    add_custom_command(
        COMMAND ${CMAKE_COMMAND} -E tar "cf" "${output_file}" --format=zip -- ${input_files}
        WORKING_DIRECTORY "${working_dir}"
        OUTPUT  "${output_file}"
        DEPENDS ${input_files}
        COMMENT "Zipping to ${output_file}."
    )
endfunction()

file(GLOB ZIP_FILES "${CMAKE_SOURCE_DIR}/editor/assets/*")
create_zip("${ZIP_FILE}" "${ZIP_FILES}" "${CMAKE_SOURCE_DIR}/editor/assets")

add_custom_target("project-data" ALL DEPENDS "${ZIP_FILE}")

## Include subdirectories

add_subdirectory("submodules/glfw")
add_subdirectory("submodules/glslang")
add_subdirectory("submodules/physfs")
