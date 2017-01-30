
# 
# External dependencies
# 

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(cppexpose REQUIRED)
find_package(cppassist REQUIRED)


# 
# Library name and options
# 

# Target name
set(target gloperate)

# Exit here if required dependencies are not met
message(STATUS "Lib ${target}")

# Set API export file and macro
string(MAKE_C_IDENTIFIER ${target} target_id)
string(TOUPPER ${target_id} target_id)
set(feature_file "include/${target}/${target}_features.h")
set(export_file  "include/${target}/${target}_api.h")
set(export_macro "${target_id}_API")


# 
# Sources
# 

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(headers
    ${include_path}/gloperate.h

    ${include_path}/base/Environment.h
    ${include_path}/base/System.h
    ${include_path}/base/ComponentManager.h
    ${include_path}/base/ResourceManager.h
    ${include_path}/base/ResourceManager.inl
    ${include_path}/base/AbstractCanvas.h
    ${include_path}/base/Canvas.h
    ${include_path}/base/AbstractContext.h
    ${include_path}/base/AbstractGLContext.h
    ${include_path}/base/AbstractGLContextFactory.h
    ${include_path}/base/GLContextFormat.h
    ${include_path}/base/GLContextUtils.h
    ${include_path}/base/CachedValue.h
    ${include_path}/base/CachedValue.inl
    ${include_path}/base/ChronoTimer.h
    ${include_path}/base/AutoTimer.h
    ${include_path}/base/TimeManager.h
    ${include_path}/base/Timer.h
    ${include_path}/base/ScriptTimer.h
    ${include_path}/base/Image.h
    ${include_path}/base/AbstractLoader.h
    ${include_path}/base/AbstractStorer.h
    ${include_path}/base/Loader.h
    ${include_path}/base/Loader.inl
    ${include_path}/base/Storer.h
    ${include_path}/base/Storer.inl
    ${include_path}/base/ExtendedProperties.h
    ${include_path}/base/AbstractColorGradient.h
    ${include_path}/base/AbstractColorGradient.inl
    ${include_path}/base/Color.h
    ${include_path}/base/ColorGradientList.h
    ${include_path}/base/ColorGradientList.inl
    ${include_path}/base/ColorGradientPreparation.h
    ${include_path}/base/ExplicitEquidistantColorGradient.h
    ${include_path}/base/ExplicitEquidistantColorGradient.inl

    ${include_path}/pipeline/PipelineContainer.h
    ${include_path}/pipeline/Stage.h
    ${include_path}/pipeline/Stage.inl
    ${include_path}/pipeline/Pipeline.h
    ${include_path}/pipeline/AbstractSlot.h
    ${include_path}/pipeline/Slot.h
    ${include_path}/pipeline/Slot.inl
    ${include_path}/pipeline/Input.h
    ${include_path}/pipeline/Input.inl
    ${include_path}/pipeline/Output.h
    ${include_path}/pipeline/Output.inl
    ${include_path}/pipeline/StageComponent.h
    ${include_path}/pipeline/StageComponent.inl

    ${include_path}/rendering/AbstractDrawable.h
    ${include_path}/rendering/Camera.h
    ${include_path}/rendering/Drawable.h
    ${include_path}/rendering/Drawable.inl
    ${include_path}/rendering/NoiseTexture.h
    ${include_path}/rendering/RenderPass.h
    ${include_path}/rendering/RenderTarget.h
    ${include_path}/rendering/RenderTargetType.h
    ${include_path}/rendering/TransparencyMasksGenerator.h

    ${include_path}/stages/interfaces/RenderInterface.h
    ${include_path}/stages/base/BasicFramebufferStage.h
    ${include_path}/stages/base/CustomFramebufferStage.h
    ${include_path}/stages/base/FramebufferStage.h
    ${include_path}/stages/base/TextureStage.h
    ${include_path}/stages/base/TextureLoadStage.h
    ${include_path}/stages/base/ProceduralTextureStage.h
    ${include_path}/stages/base/MixerStage.h
    ${include_path}/stages/base/SplitStage.h
    ${include_path}/stages/base/BlitStage.h
    ${include_path}/stages/base/ColorGradientSelectionStage.h
    ${include_path}/stages/base/ColorGradientTextureStage.h
    ${include_path}/stages/demos/DemoStage.h
    ${include_path}/stages/demos/DemoPipeline.h
    ${include_path}/stages/demos/DemoAntialiasableTriangleStage.h
    ${include_path}/stages/demos/DemoAntialiasingPipeline.h
    ${include_path}/stages/demos/DemoDOFCubeStage.h
    ${include_path}/stages/demos/DemoDOFPipeline.h
    ${include_path}/stages/demos/DemoSSAORenderingStage.h
    ${include_path}/stages/demos/DemoSSAOPostprocessingStage.h
    ${include_path}/stages/demos/DemoSSAOPipeline.h
    ${include_path}/stages/demos/DemoTransparencyStage.h
    ${include_path}/stages/demos/DemoTransparencyPipeline.h
    ${include_path}/stages/demos/DemoMultiFramePipeline.h
    ${include_path}/stages/demos/SpinningRectStage.h
    ${include_path}/stages/demos/TimerStage.h
    ${include_path}/stages/demos/ColorizeStage.h
    ${include_path}/stages/multiframe/MultiFrameAggregationPipeline.h
    ${include_path}/stages/multiframe/MultiFrameAggregationStage.h
    ${include_path}/stages/multiframe/MultiFrameControlStage.h
    ${include_path}/stages/multiframe/MultiFrameDiscDistributionStage.h
    ${include_path}/stages/multiframe/NoiseKernelStage.h
    ${include_path}/stages/multiframe/SSAOKernelStage.h
    ${include_path}/stages/multiframe/SubpixelAntialiasingOffsetStage.h
    ${include_path}/stages/multiframe/TransparencyKernelStage.h

    ${include_path}/input/constants.h
    ${include_path}/input/InputManager.h
    ${include_path}/input/InputManager.h
    ${include_path}/input/AbstractEventConsumer.h
    ${include_path}/input/AbstractDeviceProvider.h
    ${include_path}/input/AbstractDevice.h
    ${include_path}/input/InputEvent.h
    ${include_path}/input/MouseEvent.h
    ${include_path}/input/ButtonEvent.h
    ${include_path}/input/AxisEvent.h
    ${include_path}/input/MouseDevice.h
    ${include_path}/input/KeyboardDevice.h

    ${include_path}/tools/AbstractVideoExporter.h
    ${include_path}/tools/ImageExporter.h
)

set(sources
    ${source_path}/gloperate.cpp

    ${source_path}/base/Environment.cpp
    ${source_path}/base/System.cpp
    ${source_path}/base/ComponentManager.cpp
    ${source_path}/base/ResourceManager.cpp
    ${source_path}/base/AbstractCanvas.cpp
    ${source_path}/base/Canvas.cpp
    ${source_path}/base/AbstractContext.cpp
    ${source_path}/base/AbstractGLContext.cpp
    ${source_path}/base/AbstractGLContextFactory.cpp
    ${source_path}/base/GLContextFormat.cpp
    ${source_path}/base/GLContextUtils.cpp
    ${source_path}/base/ChronoTimer.cpp
    ${source_path}/base/AutoTimer.cpp
    ${source_path}/base/TimeManager.cpp
    ${source_path}/base/Timer.cpp
    ${source_path}/base/ScriptTimer.cpp
    ${source_path}/base/Image.cpp
    ${source_path}/base/AbstractLoader.cpp
    ${source_path}/base/AbstractStorer.cpp
    ${source_path}/base/AbstractColorGradient.cpp
    ${source_path}/base/Color.cpp
    ${source_path}/base/ColorGradientList.cpp
    ${source_path}/base/ColorGradientPreparation.cpp
    ${source_path}/base/ExplicitEquidistantColorGradient.cpp

    ${source_path}/pipeline/PipelineContainer.cpp
    ${source_path}/pipeline/Stage.cpp
    ${source_path}/pipeline/Pipeline.cpp
    ${source_path}/pipeline/AbstractSlot.cpp
    ${source_path}/pipeline/StageComponent.cpp

    ${source_path}/rendering/AbstractDrawable.cpp
    ${source_path}/rendering/Camera.cpp
    ${source_path}/rendering/Drawable.cpp
    ${source_path}/rendering/NoiseTexture.cpp
    ${source_path}/rendering/RenderPass.cpp
    ${source_path}/rendering/RenderTarget.cpp
    ${source_path}/rendering/TransparencyMasksGenerator.cpp

    ${source_path}/stages/interfaces/RenderInterface.cpp
    ${source_path}/stages/base/BasicFramebufferStage.cpp
    ${source_path}/stages/base/CustomFramebufferStage.cpp
    ${source_path}/stages/base/ColorGradientSelectionStage.cpp
    ${source_path}/stages/base/ColorGradientTextureStage.cpp
    ${source_path}/stages/base/FramebufferStage.cpp
    ${source_path}/stages/base/TextureStage.cpp
    ${source_path}/stages/base/TextureLoadStage.cpp
    ${source_path}/stages/base/ProceduralTextureStage.cpp
    ${source_path}/stages/base/MixerStage.cpp
    ${source_path}/stages/base/SplitStage.cpp
    ${source_path}/stages/base/BlitStage.cpp
    ${source_path}/stages/demos/DemoStage.cpp
    ${source_path}/stages/demos/DemoPipeline.cpp
    ${source_path}/stages/demos/DemoAntialiasableTriangleStage.cpp
    ${source_path}/stages/demos/DemoAntialiasingPipeline.cpp
    ${source_path}/stages/demos/DemoDOFCubeStage.cpp
    ${source_path}/stages/demos/DemoDOFPipeline.cpp
    ${source_path}/stages/demos/DemoSSAORenderingStage.cpp
    ${source_path}/stages/demos/DemoSSAOPostprocessingStage.cpp
    ${source_path}/stages/demos/DemoSSAOPipeline.cpp
    ${source_path}/stages/demos/DemoTransparencyStage.cpp
    ${source_path}/stages/demos/DemoTransparencyPipeline.cpp
    ${source_path}/stages/demos/DemoMultiFramePipeline.cpp
    ${source_path}/stages/demos/SpinningRectStage.cpp
    ${source_path}/stages/demos/TimerStage.cpp
    ${source_path}/stages/demos/ColorizeStage.cpp
    ${source_path}/stages/multiframe/MultiFrameAggregationStage.cpp
    ${source_path}/stages/multiframe/MultiFrameControlStage.cpp
    ${source_path}/stages/multiframe/MultiFrameAggregationPipeline.cpp
    ${source_path}/stages/multiframe/MultiFrameDiscDistributionStage.cpp
    ${source_path}/stages/multiframe/NoiseKernelStage.cpp
    ${source_path}/stages/multiframe/SSAOKernelStage.cpp
    ${source_path}/stages/multiframe/SubpixelAntialiasingOffsetStage.cpp
    ${source_path}/stages/multiframe/TransparencyKernelStage.cpp

    ${source_path}/input/InputManager.cpp
    ${source_path}/input/AbstractEventConsumer.cpp
    ${source_path}/input/AbstractDeviceProvider.cpp
    ${source_path}/input/AbstractDevice.cpp
    ${source_path}/input/InputEvent.cpp
    ${source_path}/input/MouseEvent.cpp
    ${source_path}/input/ButtonEvent.cpp
    ${source_path}/input/AxisEvent.cpp
    ${source_path}/input/MouseDevice.cpp
    ${source_path}/input/KeyboardDevice.cpp

    ${source_path}/tools/AbstractVideoExporter.cpp
    ${source_path}/tools/ImageExporter.cpp
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
    cpplocate::cpplocate
    cppexpose::cppexpose
    cppassist::cppassist
    glbinding::glbinding
    globjects::globjects

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
