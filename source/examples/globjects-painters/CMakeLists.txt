
# 
# External dependencies
# 

find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(libzeug REQUIRED)


# 
# Library name and options
# 

# Target name
set(target globjects-painters)

# Exit here if required dependencies are not met
message(STATUS "Plugin ${target}")


# 
# Sources
# 

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}")

set(headers
    ${include_path}/logo/Logo.h
    ${include_path}/screenaligned/ScreenAligned.h
    ${include_path}/postprocessing/Postprocessing.h
    ${include_path}/postprocessing/PostprocessingPipeline.h
    ${include_path}/cubescape/CubeScape.h
)

set(sources
    ${source_path}/logo/Logo.cpp
    ${source_path}/screenaligned/ScreenAligned.cpp
    ${source_path}/cubescape/CubeScape.cpp
    ${source_path}/postprocessing/Postprocessing.cpp
    ${source_path}/postprocessing/PostprocessingPipeline.cpp
    ${source_path}/plugin.cpp
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
)


# 
# Include directories
# 

target_include_directories(${target}
    PRIVATE
    ${PROJECT_BINARY_DIR}/source/include
    ${CMAKE_CURRENT_SOURCE_DIR}

    PUBLIC
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${GLM_INCLUDE_DIR}
    ${GLBINDING_INCLUDE_DIRS}
    ${GLOBJECTS_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/source/gloperate/include

    INTERFACE
)


# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE
    ${GLBINDING_LIBRARIES}
    ${GLOBJECTS_LIBRARIES}
    libzeug::iozeug
    libzeug::reflectionzeug
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

# Plugin library (release)
#install(TARGETS ${target}
#    RUNTIME DESTINATION ${INSTALL_PLUGINS}       CONFIGURATIONS Release
#    LIBRARY DESTINATION ${INSTALL_PLUGINS}       CONFIGURATIONS Release
#    ARCHIVE DESTINATION ${INSTALL_LIB}
#)

# Plugin library (debug)
#install(TARGETS ${target}
#    RUNTIME DESTINATION ${INSTALL_PLUGINS_DEBUG} CONFIGURATIONS Debug
#    LIBRARY DESTINATION ${INSTALL_PLUGINS_DEBUG} CONFIGURATIONS Debug
#)
