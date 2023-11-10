# toolchain-mingw64.cmake

# Set the system name to Windows to specify the target system
set(CMAKE_SYSTEM_NAME Windows)

# Specify the cross compilers
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# Where to look for the target environment. (Compiler, libraries, headers, etc.)
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# Adjust the default behaviour of the FIND_XXX() commands:
# search programs in the host environment and
# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Vulkan specific setup, adjust the paths for your system
# Assuming Vulkan SDK is installed at C:/VulkanSDK
set(Vulkan_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/crosscompile/vulkan-sdk/1.3.268.0/Include")
set(Vulkan_LIBRARY "${CMAKE_SOURCE_DIR}/crosscompile/vulkan-sdk/1.3.268.0/Lib/vulkan-1.lib")

# Then you can do something like the following to inform CMake of the location of the Qt libraries
set(Qt5Core_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/crosscompile/qt5/mingw81_64/include/QtCore")
set(Qt5Gui_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/crosscompile/qt5/mingw81_64/include/QtGui")
set(Qt5Widgets_INCLUDE_DIRS "C:/Qt/5.15/mingw81_64/include/QtWidgets")
set(Qt5Core_LIBRARY "C:/Qt/5.15/mingw81_64/lib/Qt5Core.dll")
set(Qt5Gui_LIBRARY "C:/Qt/5.15/mingw81_64/lib/Qt5Gui.dll")
set(Qt5Widgets_LIBRARY "C:/Qt/5.15/mingw81_64/lib/Qt5Widgets.dll")
