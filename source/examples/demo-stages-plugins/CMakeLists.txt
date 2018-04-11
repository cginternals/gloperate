
#
# External dependencies
#

find_package(OpenGL    REQUIRED)
find_package(glm       REQUIRED)
find_package(ASSIMP    REQUIRED)
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
set(target demo-stages-plugins)

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
    ${include_path}/AntialiasableTriangleStage.h
    ${include_path}/AntialiasingRenderingPipeline.h
    ${include_path}/AssimpMeshLoader.h
    ${include_path}/ColorGradientDemo.h
    ${include_path}/DemoAntialiasingAggregationPipeline.h
    ${include_path}/DemoDOFAggregationPipeline.h
    ${include_path}/DemoMultiFrameAggregationPipeline.h
    ${include_path}/DemoSSAOAggregationPipeline.h
    ${include_path}/DemoTextRenderingPipeline.h
    ${include_path}/DemoTransparencyAggregationPipeline.h
    ${include_path}/DOFCubeStage.h
    ${include_path}/DOFRenderingPipeline.h
    ${include_path}/GlyphSequenceDemoStage.h
    ${include_path}/LightTestPipeline.h
    ${include_path}/LightTestStage.h
    ${include_path}/MultiFramePostprocessingStage.h
    ${include_path}/MultiFrameRenderingPipeline.h
    ${include_path}/MultiFrameSceneRenderingStage.h
    ${include_path}/ShaderDemoPipeline.h
    ${include_path}/ShapeDemo.h
    ${include_path}/SSAOApplicationStage.h
    ${include_path}/SSAORenderingPipeline.h
    ${include_path}/SSAOSceneRenderingStage.h
    ${include_path}/TransparencyRenderingPipeline.h
    ${include_path}/TransparentCirclesStage.h
)

set(sources
    ${source_path}/AntialiasableTriangleStage.cpp
    ${source_path}/AntialiasingRenderingPipeline.cpp
    ${source_path}/AssimpMeshLoader.cpp
    ${source_path}/ColorGradientDemo.cpp
    ${source_path}/DemoAntialiasingAggregationPipeline.cpp
    ${source_path}/DemoDOFAggregationPipeline.cpp
    ${source_path}/DemoMultiFrameAggregationPipeline.cpp
    ${source_path}/DemoSSAOAggregationPipeline.cpp
    ${source_path}/DemoTextRenderingPipeline.cpp
    ${source_path}/DemoTransparencyAggregationPipeline.cpp
    ${source_path}/DOFCubeStage.cpp
    ${source_path}/DOFRenderingPipeline.cpp
    ${source_path}/GlyphSequenceDemoStage.cpp
    ${source_path}/LightTestPipeline.cpp
    ${source_path}/LightTestStage.cpp
    ${source_path}/MultiFramePostprocessingStage.cpp
    ${source_path}/MultiFrameRenderingPipeline.cpp
    ${source_path}/MultiFrameSceneRenderingStage.cpp
    ${source_path}/ShaderDemoPipeline.cpp
    ${source_path}/ShapeDemo.cpp
    ${source_path}/SSAOApplicationStage.cpp
    ${source_path}/SSAORenderingPipeline.cpp
    ${source_path}/SSAOSceneRenderingStage.cpp
    ${source_path}/TransparencyRenderingPipeline.cpp
    ${source_path}/TransparentCirclesStage.cpp
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
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${PROJECT_BINARY_DIR}/source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${ASSIMP_INCLUDE_DIR}

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
    ${ASSIMP_LIBRARIES}
    ${META_PROJECT_NAME}::gloperate
    ${META_PROJECT_NAME}::gloperate-glkernel
    ${META_PROJECT_NAME}::gloperate-text
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
