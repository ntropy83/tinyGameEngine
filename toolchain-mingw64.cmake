# Set the system name to Windows to specify the target system
set(CMAKE_SYSTEM_NAME Windows)
set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)

# Specify the cross compilers
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# Where to look for the target environment. (Compiler, libraries, headers, etc.)
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SOURCE_DIR}/crosscompile/vulkan-sdk/1.3.268.0)

# Dll Path
set(QT_DLL_DIR "${CMAKE_SOURCE_DIR}/crosscompile/qt6/6.6.0/mingw_64/bin")
set(MINGW_DLL_DIR "/usr/x86_64-w64-mingw32/bin")

# Adjust the default behaviour of the FIND_XXX() commands:
# search programs in the host environment and
# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set Vulkan and Qt paths
set(VULKAN_ROOT "${CMAKE_SOURCE_DIR}/crosscompile/vulkan-sdk/1.3.268.0")
set(Qt6_DIR "${CMAKE_SOURCE_DIR}/crosscompile/qt6/6.6.0/mingw_64/lib/cmake/Qt6")

# Set include and library directories
set(Vulkan_INCLUDE_DIRS "${VULKAN_ROOT}/Include")
set(Vulkan_LIBRARY "${VULKAN_ROOT}/Lib/vulkan-1.lib")

# Inform CMake to use these paths for finding packages
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${VULKAN_ROOT}" "${Qt6_DIR}")
