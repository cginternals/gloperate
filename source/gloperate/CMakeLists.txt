
# 
# External dependencies
# 

find_package(OpenGL     REQUIRED)
find_package(glm        REQUIRED)
find_package(glbinding  REQUIRED)
find_package(globjects  REQUIRED)
find_package(cppexpose  REQUIRED)
find_package(cppassist  REQUIRED)
find_package(qmltoolbox REQUIRED)
find_package(cpplocate  REQUIRED)


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
    ${include_path}/gloperate.h

    ${include_path}/base/Environment.h
    ${include_path}/base/System.h
    ${include_path}/base/TimerManager.h
    ${include_path}/base/ComponentManager.h
    ${include_path}/base/Component.h
    ${include_path}/base/Component.inl
    ${include_path}/base/ResourceManager.h
    ${include_path}/base/ResourceManager.inl
    ${include_path}/base/AbstractComponent.h
    ${include_path}/base/AbstractComponent.inl
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
    ${include_path}/base/AbstractLoader.h
    ${include_path}/base/AbstractStorer.h
    ${include_path}/base/Loader.h
    ${include_path}/base/Loader.inl
    ${include_path}/base/Storer.h
    ${include_path}/base/Storer.inl
    ${include_path}/base/Range.h
    ${include_path}/base/ExtendedProperties.h
    ${include_path}/base/ExtendedProperties.inl

    ${include_path}/pipeline/Stage.h
    ${include_path}/pipeline/Stage.inl
    ${include_path}/pipeline/Pipeline.h
    ${include_path}/pipeline/AbstractSlot.h
    ${include_path}/pipeline/AbstractSlot.inl
    ${include_path}/pipeline/Slot.h
    ${include_path}/pipeline/Slot.inl
    ${include_path}/pipeline/Input.h
    ${include_path}/pipeline/Input.inl
    ${include_path}/pipeline/Output.h
    ${include_path}/pipeline/Output.inl

    ${include_path}/rendering/AbstractDrawable.h
    ${include_path}/rendering/AttachmentType.h
    ${include_path}/rendering/Camera.h
    ${include_path}/rendering/CameraUtils.h
    ${include_path}/rendering/Drawable.h
    ${include_path}/rendering/Drawable.inl
    ${include_path}/rendering/NoiseTexture.h
    ${include_path}/rendering/RenderPass.h
    ${include_path}/rendering/LightType.h
    ${include_path}/rendering/Light.h
    ${include_path}/rendering/AbstractRenderTarget.h
    ${include_path}/rendering/ColorRenderTarget.h
    ${include_path}/rendering/DepthRenderTarget.h
    ${include_path}/rendering/DepthStencilRenderTarget.h
    ${include_path}/rendering/StencilRenderTarget.h
    ${include_path}/rendering/RenderTargetType.h
    ${include_path}/rendering/TransparencyMasksGenerator.h
    ${include_path}/rendering/ScreenAlignedQuad.h
    ${include_path}/rendering/ScreenAlignedTriangle.h
    ${include_path}/rendering/Shape.h
    ${include_path}/rendering/ShapeType.h
    ${include_path}/rendering/Point.h
    ${include_path}/rendering/Triangle.h
    ${include_path}/rendering/Quad.h
    ${include_path}/rendering/Box.h
    ${include_path}/rendering/Sphere.h
    ${include_path}/rendering/Icosahedron.h
    ${include_path}/rendering/Color.h
    ${include_path}/rendering/Image.h
    ${include_path}/rendering/AbstractColorGradient.h
    ${include_path}/rendering/AbstractColorGradient.inl
    ${include_path}/rendering/LinearColorGradient.h
    ${include_path}/rendering/LinearColorGradient.inl
    ${include_path}/rendering/ColorGradientList.h
    ${include_path}/rendering/ColorGradientList.inl

    ${include_path}/stages/interfaces/RenderInterface.h
    ${include_path}/stages/interfaces/CanvasInterface.h
    ${include_path}/stages/base/FloatSelectionStage.h
    ${include_path}/stages/base/BasicFramebufferStage.h
    ${include_path}/stages/base/TextureRenderTargetStage.h
    ${include_path}/stages/base/RenderbufferRenderTargetStage.h
    ${include_path}/stages/base/TextureLoadStage.h
    ${include_path}/stages/base/BlitStage.h
    ${include_path}/stages/base/ColorGradientStage.h
    ${include_path}/stages/base/ColorGradientSelectionStage.h
    ${include_path}/stages/base/ColorGradientTextureStage.h
    ${include_path}/stages/base/ShaderStage.h
    ${include_path}/stages/base/RenderPassStage.h
    ${include_path}/stages/base/RasterizationStage.h
    ${include_path}/stages/base/ClearStage.h
    ${include_path}/stages/base/ProgramStage.h
    ${include_path}/stages/base/ShapeStage.h
    ${include_path}/stages/base/TransformStage.h
    ${include_path}/stages/base/TimerStage.h
    ${include_path}/stages/base/TextureFromRenderTargetExtractionStage.h
    ${include_path}/stages/base/ViewportScaleStage.h
    ${include_path}/stages/navigation/TrackballStage.h
    ${include_path}/stages/lights/LightCreationStage.h
    ${include_path}/stages/lights/LightBufferTextureStage.h

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
    ${include_path}/input/PrintLineConsumer.h

    ${include_path}/tools/AbstractVideoExporter.h
    ${include_path}/tools/ImageExporter.h

    ${include_path}/loaders/ColorGradientLoader.h
    ${include_path}/loaders/ShaderLoader.h
    ${include_path}/loaders/GlrawTextureLoader.h
    ${include_path}/loaders/RawFileNameSuffix.h
)

set(sources
    ${source_path}/gloperate.cpp

    ${source_path}/base/Environment.cpp
    ${source_path}/base/System.cpp
    ${source_path}/base/TimerManager.cpp
    ${source_path}/base/ComponentManager.cpp
    ${source_path}/base/ResourceManager.cpp
    ${source_path}/base/Canvas.cpp
    ${source_path}/base/AbstractContext.cpp
    ${source_path}/base/AbstractComponent.cpp
    ${source_path}/base/AbstractGLContext.cpp
    ${source_path}/base/AbstractGLContextFactory.cpp
    ${source_path}/base/GLContextFormat.cpp
    ${source_path}/base/GLContextUtils.cpp
    ${source_path}/base/ChronoTimer.cpp
    ${source_path}/base/AutoTimer.cpp
    ${source_path}/base/AbstractLoader.cpp
    ${source_path}/base/AbstractStorer.cpp
    ${source_path}/base/Range.cpp
    ${source_path}/base/ExtendedProperties.cpp

    ${source_path}/pipeline/Stage.cpp
    ${source_path}/pipeline/Pipeline.cpp
    ${source_path}/pipeline/AbstractSlot.cpp

    ${source_path}/rendering/AbstractDrawable.cpp
    ${source_path}/rendering/Camera.cpp
    ${source_path}/rendering/CameraUtils.cpp
    ${source_path}/rendering/Drawable.cpp
    ${source_path}/rendering/NoiseTexture.cpp
    ${source_path}/rendering/RenderPass.cpp
    ${source_path}/rendering/AbstractRenderTarget.cpp
    ${source_path}/rendering/ColorRenderTarget.cpp
    ${source_path}/rendering/DepthRenderTarget.cpp
    ${source_path}/rendering/DepthStencilRenderTarget.cpp
    ${source_path}/rendering/StencilRenderTarget.cpp
    ${source_path}/rendering/TransparencyMasksGenerator.cpp
    ${source_path}/rendering/ScreenAlignedQuad.cpp
    ${source_path}/rendering/ScreenAlignedTriangle.cpp
    ${source_path}/rendering/Shape.cpp
    ${source_path}/rendering/Point.cpp
    ${source_path}/rendering/Triangle.cpp
    ${source_path}/rendering/Quad.cpp
    ${source_path}/rendering/Box.cpp
    ${source_path}/rendering/Sphere.cpp
    ${source_path}/rendering/Icosahedron.cpp
    ${source_path}/rendering/Color.cpp
    ${source_path}/rendering/Image.cpp
    ${source_path}/rendering/AbstractColorGradient.cpp
    ${source_path}/rendering/LinearColorGradient.cpp
    ${source_path}/rendering/ColorGradientList.cpp

    ${source_path}/stages/interfaces/RenderInterface.cpp
    ${source_path}/stages/interfaces/CanvasInterface.cpp
    ${source_path}/stages/base/FloatSelectionStage.cpp
    ${source_path}/stages/base/BasicFramebufferStage.cpp
    ${source_path}/stages/base/ColorGradientStage.cpp
    ${source_path}/stages/base/ColorGradientSelectionStage.cpp
    ${source_path}/stages/base/ColorGradientTextureStage.cpp
    ${source_path}/stages/base/ShaderStage.cpp
    ${source_path}/stages/base/RenderPassStage.cpp
    ${source_path}/stages/base/RasterizationStage.cpp
    ${source_path}/stages/base/ClearStage.cpp
    ${source_path}/stages/base/ProgramStage.cpp
    ${source_path}/stages/base/TextureRenderTargetStage.cpp
    ${source_path}/stages/base/RenderbufferRenderTargetStage.cpp
    ${source_path}/stages/base/TextureLoadStage.cpp
    ${source_path}/stages/base/BlitStage.cpp
    ${source_path}/stages/base/ShapeStage.cpp
    ${source_path}/stages/base/TransformStage.cpp
    ${source_path}/stages/base/TimerStage.cpp
    ${source_path}/stages/base/TextureFromRenderTargetExtractionStage.cpp
    ${source_path}/stages/base/ViewportScaleStage.cpp
    ${source_path}/stages/navigation/TrackballStage.cpp
    ${source_path}/stages/lights/LightCreationStage.cpp
    ${source_path}/stages/lights/LightBufferTextureStage.cpp

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
    ${source_path}/input/PrintLineConsumer.cpp

    ${source_path}/tools/AbstractVideoExporter.cpp
    ${source_path}/tools/ImageExporter.cpp

    ${source_path}/loaders/ColorGradientLoader.cpp
    ${source_path}/loaders/ShaderLoader.cpp
    ${source_path}/loaders/GlrawTextureLoader.cpp
    ${source_path}/loaders/RawFileNameSuffix.cpp
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

# Check for availability of module; use pre-generated version if not found
if (WriterCompilerDetectionHeaderFound)
    write_compiler_detection_header(
        FILE ${feature_file}
        PREFIX ${target_id}
        COMPILERS AppleClang Clang GNU MSVC
        FEATURES cxx_noexcept
        VERSION 3.2
    )
else()
    file(
        COPY ${PROJECT_SOURCE_DIR}/source/codegeneration/${target}_features.h
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/${target}
        USE_SOURCE_PERMISSIONS
    )
endif()

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
