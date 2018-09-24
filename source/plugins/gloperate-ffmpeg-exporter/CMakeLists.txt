
#
# External dependencies
#

find_package(glm REQUIRED)
find_package(cpplocate REQUIRED)
find_package(cppassist REQUIRED)
find_package(cppexpose REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)

# FFMPEG
find_package(FFMPEG)

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
set(target gloperate-ffmpeg-exporter-plugins)

# Exit here if required dependencies are not met
if (NOT FFMPEG_FOUND)
    message("Lib ${target} skipped: FFMPEG not found")
    return()
else()
    message(STATUS "Lib ${target}")
endif()


#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}")

set(headers
    ${include_path}/FFMPEGVideoEncoder.h
    ${include_path}/FFMPEGVideoExporter.h
)

set(sources
    ${source_path}/FFMPEGVideoEncoder.cpp
    ${source_path}/FFMPEGVideoExporter.cpp
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
add_library(${target} SHARED
    ${sources}
    ${headers}
)

# Create namespaced alias
add_library(${META_PROJECT_NAME}::${target} ALIAS ${target})

# Export library for downstream projects
export(TARGETS ${target} NAMESPACE ${META_PROJECT_NAME}:: FILE ${PROJECT_BINARY_DIR}/cmake/${target}/${target}-export.cmake)


#
# Project options
#

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
    DEBUG_POSTFIX "-debug"
)


#
# Include directories
#

target_include_directories(${target}
    PRIVATE
    ${FFMPEG_INCLUDE_DIR}
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${PROJECT_BINARY_DIR}/source/include
    ${CMAKE_CURRENT_SOURCE_DIR}

    PUBLIC

    INTERFACE
)


#
# Libraries
#

target_link_libraries(${target}
    PRIVATE
    cpplocate::cpplocate
    cppassist::cppassist
    cppexpose::cppexpose
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
    $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:${target_upper}_STATIC_DEFINE>
    ${DEFAULT_COMPILE_DEFINITIONS}

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
    ${FFMPEG_LIBRARIES}

    PUBLIC
    ${DEFAULT_LINKER_OPTIONS}

    INTERFACE
)


#
# Deployment
#

# Plugin library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_PLUGINS} COMPONENT runtime
    LIBRARY DESTINATION ${INSTALL_PLUGINS} COMPONENT runtime
)
