
# 
# External dependencies
# 

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(libzeug REQUIRED)
find_package(ASSIMP)

# Qt5
find_package(Qt5OpenGL  5.1)
find_package(Qt5Core    5.1)
find_package(Qt5Gui     5.1)
find_package(Qt5Widgets 5.1)

# Enable automoc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)
set_property(GLOBAL PROPERTY AUTOMOC_FOLDER CMakeAutomocTargets)

# ENABLE CMP0020: Automatically link Qt executables to qtmain target on Windows.
if (POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()


# 
# Library name and options
# 

# Target name
set(target gloperate-qt)
set(target_upper GLOPERATE_QT)

# Exit here if required dependencies are not met
if (NOT Qt5Core_FOUND)
    message("Lib ${target} skipped: Qt5 not found")
    return()
else()
    message(STATUS "Lib ${target}")
endif()

# Set API export file and macro
set(feature_file "include/${target}/${target}_features.h")
set(export_file  "include/${target}/${target}_api.h")
set(export_macro "${target_upper}_API")


# 
# Sources
# 

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(headers
    ${include_path}/viewer/Application.h
    ${include_path}/viewer/DefaultMapping.h
    ${include_path}/viewer/Viewer.h
    ${include_path}/viewer/QtEventTransformer.h
    ${include_path}/viewer/QtKeyEventProvider.h
    ${include_path}/viewer/QtMouseEventProvider.h
    ${include_path}/viewer/QtOpenGLWindow.h
    ${include_path}/viewer/QtOpenGLWindowBase.h
    ${include_path}/viewer/QtTextureLoader.h
    ${include_path}/viewer/QtTextureStorer.h
    ${include_path}/viewer/QtWheelEventProvider.h
    ${include_path}/viewer/Converter.h
    ${include_path}/viewer/TimePropagator.h
    ${include_path}/viewer/AbstractQtEventProvider.h
    ${include_path}/viewer/QtFrameEventProvider.h

    ${include_path}/scripting/ScriptEnvironment.h
    ${include_path}/scripting/SystemApi.h
    ${include_path}/scripting/TimerApi.h
    ${include_path}/scripting/ViewerApi.h
    ${include_path}/scripting/PluginApi.h

    ${include_path}/widgets/FileNameTextEdit.h
    ${include_path}/widgets/ImageExporterWidget.h
    ${include_path}/widgets/ImageExporterMultiFrameWidget.h
    ${include_path}/widgets/ImageExporterOutputWidget.h
    ${include_path}/widgets/ImageExporterResolutionWidget.h
    ${include_path}/widgets/ImageExporterTilebasedWidget.h
    ${include_path}/widgets/PluginPathWidget.h
    ${include_path}/widgets/PluginConfigWidget.h
    ${include_path}/widgets/PluginWidget.h
)

set(sources
    ${source_path}/viewer/Application.cpp
    ${source_path}/viewer/DefaultMapping.cpp
    ${source_path}/viewer/Viewer.cpp
    ${source_path}/viewer/Viewer.ui
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
    ${source_path}/viewer/AbstractQtEventProvider.cpp
    ${source_path}/viewer/QtFrameEventProvider.cpp

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
    ${source_path}/widgets/PluginPathWidget.cpp
    ${source_path}/widgets/PluginConfigWidget.cpp
    ${source_path}/widgets/PluginWidget.cpp
    ${source_path}/widgets/PluginWidget.ui
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${headers})
source_group_by_path(${source_path}  "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})


# 
# Create library
# 

# Build library
add_library(${target}
    ${sources}
    ${headers}
)

# Create namespaced alias
add_library(${META_PROJECT_NAME}::${target} ALIAS ${target})

# Export library for downstream projects
export(TARGETS ${target} NAMESPACE ${META_PROJECT_NAME}:: FILE ${PROJECT_BINARY_DIR}/cmake/${target}/${target}-export.cmake)

# Create feature detection header
# Compilers: https://cmake.org/cmake/help/v3.1/variable/CMAKE_LANG_COMPILER_ID.html#variable:CMAKE_%3CLANG%3E_COMPILER_ID
# Feature: https://cmake.org/cmake/help/v3.1/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html
write_compiler_detection_header(
    FILE ${feature_file}
    PREFIX ${target_upper}
    COMPILERS AppleClang Clang GNU MSVC
    FEATURES cxx_alignas cxx_alignof cxx_constexpr cxx_final cxx_noexcept cxx_nullptr cxx_sizeof_member cxx_thread_local
)

# Create API export header
generate_export_header(${target}
    EXPORT_FILE_NAME  ${export_file}
    EXPORT_MACRO_NAME ${export_macro}
)


# 
# Project options
# 

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
)


# 
# Include directories
# 

target_include_directories(${target}
    PRIVATE
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${PROJECT_BINARY_DIR}/source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/source/gloperate/include
    ${PROJECT_SOURCE_DIR}/source/gloperate-assimp/include
    ${GLM_INCLUDE_DIR}

    PUBLIC

    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::OpenGL
    cpplocate::cpplocate
    libzeug::reflectionzeug
    libzeug::signalzeug
    libzeug::loggingzeug
    libzeug::scriptzeug
    libzeug::widgetzeug
    libzeug::propertyguizeug
    glbinding::glbinding
    globjects::globjects
    ${META_PROJECT_NAME}::gloperate
    $<$<BOOL:${ASSIMP_FOUND}>:${META_PROJECT_NAME}::gloperate-assimp>

    PUBLIC
    ${DEFAULT_LIBRARIES}

    INTERFACE
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE

    PUBLIC
    $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:${target_upper}_STATIC_DEFINE>
    ${DEFAULT_COMPILE_DEFINITIONS}
    $<$<BOOL:${ASSIMP_FOUND}>:GLOPERATE_ASSIMP_FOUND>
    GLM_FORCE_RADIANS

    INTERFACE
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_COMPILE_OPTIONS}

    INTERFACE
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LINKER_OPTIONS}

    INTERFACE
)


# 
# Deployment
# 

# Library
install(TARGETS ${target}
    EXPORT  "${target}-export"            COMPONENT dev
    RUNTIME DESTINATION ${INSTALL_BIN}    COMPONENT runtime
    LIBRARY DESTINATION ${INSTALL_SHARED} COMPONENT runtime
    ARCHIVE DESTINATION ${INSTALL_LIB}    COMPONENT dev
)

# Header files
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE}
    COMPONENT dev
)

# Generated header files
install(DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE}
    COMPONENT dev
)

# CMake config
install(EXPORT ${target}-export
    NAMESPACE   ${META_PROJECT_NAME}::
    DESTINATION ${INSTALL_CMAKE}/${target}
    COMPONENT   dev
)
