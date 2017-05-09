
#
# External dependencies
#

find_package(OpenGL    REQUIRED)
find_package(GLM       REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(cpplocate REQUIRED)
find_package(cppexpose REQUIRED)
find_package(cppassist REQUIRED)
find_package(glkernel  REQUIRED)


#
# Library name and options
#

# Target name
set(target demo-stages)

# Exit here if required dependencies are not met
message(STATUS "Plugin ${target}")

# Set API export file and macro
string(MAKE_C_IDENTIFIER ${target} target_id)
string(TOUPPER ${target_id} target_id)
#set(feature_file "include/${target}/${target}_features.h")
#set(export_file  "include/${target}/${target}_api.h")
#set(export_macro "${target_id}_API")


#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}")

set(headers
    ${include_path}/DemoStage.h
    ${include_path}/DemoPipeline.h
    ${include_path}/ShaderDemoPipeline.h
    ${include_path}/DemoAntialiasableTriangleStage.h
    ${include_path}/DemoAntialiasingPipeline.h
    ${include_path}/DemoDOFCubeStage.h
    ${include_path}/DemoDOFPipeline.h
    ${include_path}/DemoSSAORenderingStage.h
    ${include_path}/DemoSSAOPostprocessingStage.h
    ${include_path}/DemoSSAOPipeline.h
    ${include_path}/DemoTransparencyStage.h
    ${include_path}/DemoTransparencyPipeline.h
    ${include_path}/DemoMultiFramePipeline.h
    ${include_path}/SpinningRectStage.h
    ${include_path}/DemoRenderStage.h
    ${include_path}/DemoDrawableStage.h
    ${include_path}/TimerStage.h
    ${include_path}/ColorizeStage.h
    ${include_path}/LightTestStage.h
    ${include_path}/LightTestPipeline.h
)

set(sources
    ${source_path}/plugin.cpp

    ${source_path}/DemoStage.cpp
    ${source_path}/DemoPipeline.cpp
    ${source_path}/ShaderDemoPipeline.cpp
    ${source_path}/DemoAntialiasableTriangleStage.cpp
    ${source_path}/DemoAntialiasingPipeline.cpp
    ${source_path}/DemoDOFCubeStage.cpp
    ${source_path}/DemoDOFPipeline.cpp
    ${source_path}/DemoSSAORenderingStage.cpp
    ${source_path}/DemoSSAOPostprocessingStage.cpp
    ${source_path}/DemoSSAOPipeline.cpp
    ${source_path}/DemoTransparencyStage.cpp
    ${source_path}/DemoTransparencyPipeline.cpp
    ${source_path}/DemoMultiFramePipeline.cpp
    ${source_path}/SpinningRectStage.cpp
    ${source_path}/DemoRenderStage.cpp
    ${source_path}/DemoDrawableStage.cpp
    ${source_path}/TimerStage.cpp
    ${source_path}/ColorizeStage.cpp
    ${source_path}/LightTestStage.cpp
    ${source_path}/LightTestPipeline.cpp
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
    ${GLM_INCLUDE_DIR}

    PUBLIC

    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/source/include>
    $<INSTALL_INTERFACE:include>
)


#
# Libraries
#

target_link_libraries(${target}
    PRIVATE
    ${OPENGL_LIBRARIES}
    ${META_PROJECT_NAME}::gloperate
    ${META_PROJECT_NAME}::gloperate-glkernel
    cpplocate::cpplocate
    cppexpose::cppexpose
    cppassist::cppassist
    glbinding::glbinding
    globjects::globjects
    glkernel::glkernel

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
)


#
# Deployment
#

# Library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT runtime
    BUNDLE  DESTINATION ${INSTALL_BIN} COMPONENT runtime
    LIBRARY DESTINATION ${INSTALL_LIB} COMPONENT runtime
)
