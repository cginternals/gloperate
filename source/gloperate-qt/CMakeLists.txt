
#
# External dependencies
#

# find_package(glm REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(cppexpose REQUIRED)
find_package(cppassist REQUIRED)
find_package(cpplocate REQUIRED)

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
cmake_policy(SET CMP0020 NEW)


#
# Library name and options
#

# Target name
set(target gloperate-qt)

# Exit here if required dependencies are not met
if (NOT Qt5Core_FOUND OR NOT Qt5OpenGL_FOUND)
    message("Lib ${target} skipped: Qt5 not found")
    return()
else()
    message(STATUS "Lib ${target}")
endif()

# Set API export file and macro
string(MAKE_C_IDENTIFIER ${target} target_id)
string(TOUPPER ${target_id} target_id)
set(feature_file         "include/${target}/${target}_features.h")
set(export_file          "include/${target}/${target}_export.h")
set(template_export_file "include/${target}/${target}_api.h")
set(export_macro         "${target_id}_API")


#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(headers
    ${include_path}/base/Application.h
    ${include_path}/base/OpenGLWindow.h
    ${include_path}/base/RenderWindow.h
    ${include_path}/base/GLContext.h
    ${include_path}/base/GLContextFactory.h
    ${include_path}/base/Converter.h
    ${include_path}/base/QtOpenGL.h

    ${include_path}/scripting/ECMA26251Completer.h
    ${include_path}/scripting/ECMA26251SyntaxHighlighter.h
    ${include_path}/scripting/ScriptCompleter.h
    ${include_path}/scripting/ScriptPromptWidget.h
    ${include_path}/scripting/ScriptSyntaxHighlighter.h

    ${include_path}/loaders/QtImageLoader.h
    ${include_path}/loaders/QtImageStorer.h
    ${include_path}/loaders/QtTextureLoader.h
    ${include_path}/loaders/QtTextureStorer.h
)

set(sources
    ${source_path}/base/Application.cpp
    ${source_path}/base/OpenGLWindow.cpp
    ${source_path}/base/RenderWindow.cpp
    ${source_path}/base/GLContext.cpp
    ${source_path}/base/GLContext_qt.cpp
    ${source_path}/base/GLContextFactory.cpp
    ${source_path}/base/Converter.cpp
    ${source_path}/base/QtOpenGL.cpp

    ${source_path}/scripting/ECMA26251Completer.cpp
    ${source_path}/scripting/ECMA26251SyntaxHighlighter.cpp
    ${source_path}/scripting/ScriptCompleter.cpp
    ${source_path}/scripting/ScriptPromptWidget.cpp
    ${source_path}/scripting/ScriptSyntaxHighlighter.cpp

    ${source_path}/loaders/QtTextureLoader.cpp
    ${source_path}/loaders/QtTextureStorer.cpp
    ${source_path}/loaders/QtImageLoader.cpp
    ${source_path}/loaders/QtImageStorer.cpp
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.inl$"
    ${header_group} ${headers})
source_group_by_path(${source_path}  "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.inl$"
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

# Create API export header
generate_export_header(${target}
    EXPORT_FILE_NAME  ${export_file}
    EXPORT_MACRO_NAME ${export_macro}
)

generate_template_export_header(${target}
    ${target_id}
    ${template_export_file}
)


#
# Project options
#

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
    VERSION ${META_VERSION}
    SOVERSION ${META_VERSION_MAJOR}
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
    cppexpose::cppexpose
    cppassist::cppassist
    glbinding::glbinding
    globjects::globjects
    ${META_PROJECT_NAME}::gloperate

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
    $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:${target_id}_STATIC_DEFINE>
    ${DEFAULT_COMPILE_DEFINITIONS}
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
# Target Health
#

perform_health_checks(
    ${target}
    ${sources}
    ${headers}
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
