cmake_minimum_required(VERSION 3.7)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

set(NAME tgEngine)

project(${NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/editor/src)
include_directories(${CMAKE_SOURCE_DIR}/editor/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/submodules)

# Use FindVulkan module added with CMAKE 3.7
IF (NOT CMAKE_VERSION VERSION_LESS 3.7.0)
	message(STATUS "Using module to find Vulkan")
	find_package(Vulkan)
ENDIF()

IF(UNIX AND NOT APPLE)
	set(LINUX TRUE)
    find_package(X11 REQUIRED)
ENDIF()

IF(WIN32)
    include_directories($ENV{VULKAN_SDK}/include)
    IF (NOT Vulkan_FOUND)
	    find_library(Vulkan_LIBRARY NAMES vulkan-1 vulkan PATHS ${CMAKE_SOURCE_DIR}/libs/vulkan)
	    IF (Vulkan_LIBRARY)
		    set(Vulkan_FOUND ON)
		    MESSAGE("Using bundled Vulkan library version")
        ENDIF()
    ENDIF()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WIN32_KHR")
ELSEIF(LINUX)
	IF (NOT Vulkan_FOUND)
		find_library(Vulkan_LIBRARY NAMES vulkan HINTS "$ENV{VULKAN_SDK}/lib" "${CMAKE_SOURCE_DIR}/libs/vulkan" REQUIRED)
		IF (Vulkan_LIBRARY)
			set(Vulkan_FOUND ON)
			MESSAGE("Using bundled Vulkan library version") 
		ENDIF()
	ENDIF()
    SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -O2")
    SET(GCC_COVERAGE_LINK_FLAGS    "")

	find_package(Threads REQUIRED)
ELSEIF(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_MACOS_MVK -DVK_EXAMPLE_XCODE_GENERATED")
	# Todo : android?
ENDIF(WIN32)

## Find Qt

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Widgets LinguistTools Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Gui)

set(TS_FILES editor/lang/tge_qt_gui_de_DE.ts)

set(PROJECT_SOURCES
        editor/src/debug/tge_QtDebug.cpp
        editor/src/ui/mainwindow.ui
        editor/src/main.cpp
        editor/src/ui/tge_mainwindow.cpp
        editor/include/ui/tge_mainwindow.h
        editor/src/ui/tge_vulkanwindow.cpp      
    #    ${TS_FILES}
        ## Vulkan Engine
        engine/src/file/filesystem.cpp
        engine/src/file/JsonFile.cpp
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
                        vulkan
                        glfw 
                        simdjson physfs
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
