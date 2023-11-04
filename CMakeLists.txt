cmake_minimum_required(VERSION 3.11.0)

message(STATUS "Using ${CMAKE_GENERATOR}")

if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    if (NOT MINGW_PATH) 
        message(FATAL_ERROR "MINGW_PATH not set in .env.cmake")
    endif()
    set(USE_MINGW "True")
    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
endif()

set(NAME tgEngine)

project(${NAME} VERSION 0.1 LANGUAGES CXX)

set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++20 -O2")
set(GCC_COVERAGE_LINK_FLAGS    "")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(${CMAKE_SOURCE_DIR}/editor/src)
include_directories(${CMAKE_SOURCE_DIR}/editor/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/submodules)

if (DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
  set(Vulkan_FOUND "True")
else()
  find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
  message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()

IF(UNIX AND NOT APPLE)
	set(LINUX TRUE)
    find_package(X11 REQUIRED)
ENDIF()

IF(WIN32)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WIN32_KHR")
    
ELSEIF(LINUX)

	find_package(Threads REQUIRED)
    
ELSEIF(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_MACOS_MVK -DVK_EXAMPLE_XCODE_GENERATED")
	# Todo : android?
ENDIF(WIN32)

## Find Qt

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Widgets LinguistTools Gui Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Gui Core)

set(TS_FILES editor/lang/tge_qt_gui_de_DE.ts)

set(PROJECT_SOURCES
        editor/src/debug/tge_QtDebug.cpp
        editor/src/main.cpp
        editor/include/ui/tge_mainwindow.h        
        editor/src/ui/tge_mainwindow.cpp
        #editor/include/ui/tge_settingsdialog.h
        #editor/src/ui/tge_settingsdialog.cpp
        editor/include/ui/tge_splashscreen.h
        editor/src/ui/tge_splashscreen.cpp
        editor/src/ui/qt_ui/mainwindow.ui
        #editor/src/ui/qt_ui/tge_settingsdialog.ui
        editor/src/ui/qt_ui/tge_splashscreen.ui
        editor/src/ui/tge_vulkanwindow.cpp      
    #    ${TS_FILES}
        ## Vulkan Engine
        engine/src/debug/tge_vulDebug.cpp
        engine/src/file/filesystem.cpp
        engine/src/vulkan/tge_shadercomp.cpp
        engine/src/vulkan/tge_device.cpp
        engine/src/vulkan/tge_pipeline.cpp
        engine/src/vulkan/tge_swap_chain.cpp
        engine/include/vulkan/tge_window.hpp
        engine/src/vulkan/tge_window.cpp)

add_subdirectory(engine)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET tge_qt_gui APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(${NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${NAME}
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

IF(LINUX)
    target_link_libraries(${NAME} PRIVATE ${X11_LIBRARIES})
    target_include_directories(${NAME} PRIVATE ${X11_INCLUDE_DIR})
ENDIF()

target_link_libraries(${NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Core
                        Vulkan::Vulkan
                        glfw 
                        physfs
                        glslang glslang-default-resource-limits SPIRV MachineIndependent GenericCodeGen OSDependent)

set_target_properties(${NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${NAME})
endif()
