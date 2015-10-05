
#
# Target
#
set(target gloperate-qt)


#
# External libraries
#

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(libzeug REQUIRED)
find_package(ASSIMP QUIET)


# Qt5

# good resource: http://www.kdab.com/using-cmake-with-qt-5/

# http://qt-project.org/forums/viewthread/30006/
if(MSVC)
    cmake_policy(SET CMP0020 NEW)
endif()

find_package(Qt5OpenGL  5.1 QUIET)
find_package(Qt5Core    5.1 QUIET)
find_package(Qt5Gui     5.1 QUIET)
find_package(Qt5Widgets 5.1 QUIET)

if (NOT Qt5Core_FOUND)
    message(STATUS "Lib ${target} (disabled because Qt5 couldn't be found)")
    return()
endif()

# Qt5 options

set(CMAKE_AUTOMOC ON)

if(MSVC)
    # -> msvc14 : declaration hides class member (problem in qt)
    set(DEFAULT_COMPILE_FLAGS ${DEFAULT_COMPILE_FLAGS} /wd4458)
endif()


#
# Announce target
#

message(STATUS "Lib ${target}")


#
# Includes
#

include_directories(
    ${GLM_INCLUDE_DIR}
    ${GLBINDING_INCLUDE_DIRS}
    ${GLOBJECTS_INCLUDE_DIRS}
    ${LIBZEUG_INCLUDE_DIRS}
)

include_directories(
    BEFORE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/source/gloperate/include
    ${CMAKE_SOURCE_DIR}/source/gloperate-assimp/include
)


#
# Libraries
#

set(libs
    ${GLBINDING_LIBRARIES}
    ${GLOBJECTS_LIBRARIES}
    ${LIBZEUG_LIBRARIES}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::OpenGL
    gloperate
)

if (ASSIMP_FOUND)
    list(APPEND libs
        gloperate-assimp
    )
endif ()


#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DGLOPERATE_STATIC")
else()
    add_definitions("-DGLOPERATE_QT_EXPORTS")
endif()

# Include assimp specific code
if (ASSIMP_FOUND)
   add_definitions("-DGLOPERATE_ASSIMP_FOUND")
endif()

# for compatibility between glm 0.9.4 and 0.9.5
add_definitions("-DGLM_FORCE_RADIANS")


#
# Sources
#

set(header_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(headers
    ${header_path}/viewer/Application.h
    ${header_path}/viewer/DefaultMapping.h
    ${header_path}/viewer/Viewer.h
    ${header_path}/viewer/AbstractQtMapping.h
    ${header_path}/viewer/QtEventTransformer.h
    ${header_path}/viewer/QtKeyEventProvider.h
    ${header_path}/viewer/QtMouseEventProvider.h
    ${header_path}/viewer/QtOpenGLWindow.h
    ${header_path}/viewer/QtOpenGLWindowBase.h
    ${header_path}/viewer/QtTextureLoader.h
    ${header_path}/viewer/QtTextureStorer.h
    ${header_path}/viewer/QtWheelEventProvider.h
    ${header_path}/viewer/Converter.h
    ${header_path}/viewer/TimePropagator.h

    ${header_path}/scripting/ScriptEnvironment.h
    ${header_path}/scripting/SystemApi.h
    ${header_path}/scripting/TimerApi.h
    ${header_path}/scripting/ViewerApi.h
    ${header_path}/scripting/PluginApi.h

    ${header_path}/widgets/FileNameTextEdit.h
    ${header_path}/widgets/ImageExporterWidget.h
    ${header_path}/widgets/ImageExporterMultiFrameWidget.h
    ${header_path}/widgets/ImageExporterOutputWidget.h
    ${header_path}/widgets/ImageExporterResolutionWidget.h
    ${header_path}/widgets/ImageExporterTilebasedWidget.h

    ${header_path}/gloperate-qt_api.h
)

set(sources
    ${source_path}/viewer/Application.cpp
    ${source_path}/viewer/DefaultMapping.cpp
    ${source_path}/viewer/Viewer.cpp
    ${source_path}/viewer/Viewer.ui
    ${source_path}/viewer/AbstractQtMapping.cpp
    ${source_path}/viewer/QtEventTransformer.cpp
    ${source_path}/viewer/QtKeyEventProvider.cpp
    ${source_path}/viewer/QtMouseEventProvider.cpp
    ${source_path}/viewer/QtOpenGLWindow.cpp
    ${source_path}/viewer/QtOpenGLWindowBase.cpp
    ${source_path}/viewer/QtTextureLoader.cpp
    ${source_path}/viewer/QtTextureStorer.cpp
    ${source_path}/viewer/QtWheelEventProvider.cpp
    ${source_path}/viewer/Converter.cpp
    ${source_path}/viewer/TimePropagator.cpp

    ${source_path}/scripting/ScriptEnvironment.cpp
    ${source_path}/scripting/SystemApi.cpp
    ${source_path}/scripting/TimerApi.cpp
    ${source_path}/scripting/ViewerApi.cpp
    ${source_path}/scripting/PluginApi.cpp

    ${source_path}/widgets/FileNameTextEdit.cpp
    ${source_path}/widgets/ImageExporterWidget.cpp
    ${source_path}/widgets/ImageExporterMultiFrameWidget.cpp
    ${source_path}/widgets/ImageExporterMultiFrameWidget.ui
    ${source_path}/widgets/ImageExporterOutputWidget.cpp
    ${source_path}/widgets/ImageExporterOutputWidget.ui
    ${source_path}/widgets/ImageExporterResolutionWidget.cpp
    ${source_path}/widgets/ImageExporterResolutionWidget.ui
    ${source_path}/widgets/ImageExporterTilebasedWidget.cpp
    ${source_path}/widgets/ImageExporterTilebasedWidget.ui
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${header_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${headers})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})


#
# Build library
#

list_extract(uis "\\\\.ui$" ${sources})
qt5_wrap_ui(uihs ${uis})

add_library(${target} ${headers} ${sources})
qt5_use_modules(${target} Core Gui Widgets)
target_link_libraries(${target} ${libs})

target_compile_options(${target} PRIVATE ${DEFAULT_COMPILE_FLAGS})

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      "${IDE_FOLDER}"
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH                ${header_path})


#
# Deployment
#

# Library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

# Header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE})
