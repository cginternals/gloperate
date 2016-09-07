
# 
# External dependencies
# 

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(libzeug REQUIRED)


# 
# Library name and options
# 

# Target name
set(target gloperate)

# Exit here if required dependencies are not met
message(STATUS "Lib ${target}")

# Set API export file and macro
string(TOUPPER ${target} target_upper)
set(feature_file "include/${target}/${target}_features.h")
set(export_file  "include/${target}/${target}_api.h")
set(export_macro "${target_upper}_API")


# 
# Sources
# 

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(headers
    ${include_path}/gloperate.h
    ${include_path}/offsetof.h
    
    ${include_path}/base/AbstractColorGradient.h
    ${include_path}/base/AbstractColorGradient.hpp
    ${include_path}/base/ColorGradientList.h
    ${include_path}/base/ColorGradientList.hpp
    ${include_path}/base/ExplicitEquidistantColorGradient.h
    ${include_path}/base/ExplicitEquidistantColorGradient.hpp
    ${include_path}/base/RenderTarget.h
    ${include_path}/base/registernamedstrings.h
    ${include_path}/base/collection.hpp
    ${include_path}/base/CyclicTime.h
    ${include_path}/base/ChronoTimer.h
    ${include_path}/base/AutoTimer.h
    ${include_path}/base/make_unique.hpp
    ${include_path}/base/CachedValue.h
    ${include_path}/base/CachedValue.hpp
    ${include_path}/base/AbstractWindow.h
        
    ${include_path}/input/MouseEvent.h
    ${include_path}/input/KeyboardEvent.h
    ${include_path}/input/WheelEvent.h
    ${include_path}/input/AbstractEventProvider.h
    ${include_path}/input/input.h
    ${include_path}/input/KeyboardInputHandler.h
    ${include_path}/input/MouseInputHandler.h
    ${include_path}/input/AbstractEvent.h
    
    ${include_path}/navigation/WorldInHandNavigation.h
    ${include_path}/navigation/AbstractInteraction.h
    ${include_path}/navigation/AbstractMapping.h
    ${include_path}/navigation/CoordinateProvider.h
    ${include_path}/navigation/navigationmath.h
    
    ${include_path}/painter/InputCapability.h
    ${include_path}/painter/AbstractInputCapability.h
    ${include_path}/painter/Painter.hpp
    ${include_path}/painter/PerspectiveProjectionCapability.h
    ${include_path}/painter/AbstractProjectionCapability.h
    ${include_path}/painter/ContextFormat.h
    ${include_path}/painter/Camera.h
    ${include_path}/painter/TypedRenderTargetCapability.h
    ${include_path}/painter/AbstractViewportCapability.h
    ${include_path}/painter/Painter.h
    ${include_path}/painter/AbstractTargetFramebufferCapability.h
    ${include_path}/painter/AbstractCapability.h
    ${include_path}/painter/AbstractCapability.hpp
    ${include_path}/painter/ViewportCapability.h
    ${include_path}/painter/VirtualTimeCapability.h
    ${include_path}/painter/CameraCapability.h
    ${include_path}/painter/AbstractPerspectiveProjectionCapability.h
    ${include_path}/painter/AbstractContext.h
    ${include_path}/painter/TargetFramebufferCapability.h
    ${include_path}/painter/AbstractCameraCapability.h
    ${include_path}/painter/AbstractVirtualTimeCapability.h
    ${include_path}/painter/AbstractTypedRenderTargetCapability.h
    ${include_path}/painter/AbstractMetaInformationCapability.h
    ${include_path}/painter/MetaInformationCapability.h
    ${include_path}/painter/AbstractOutputCapability.h
    ${include_path}/painter/AbstractOutputCapability.hpp
    ${include_path}/painter/PipelineOutputCapability.h
    ${include_path}/painter/AbstractOrthographicProjectionCapability.h
    ${include_path}/painter/OrthographicProjectionCapability.h
    
    ${include_path}/pipeline/AbstractData.h
    ${include_path}/pipeline/AbstractPipeline.hpp
    ${include_path}/pipeline/Data.hpp
    ${include_path}/pipeline/AbstractStage.h
    ${include_path}/pipeline/InputSlot.h
    ${include_path}/pipeline/PipelinePainter.h
    ${include_path}/pipeline/PipelinePainter.hpp
    ${include_path}/pipeline/InputSlot.hpp
    ${include_path}/pipeline/AbstractPipeline.h
    ${include_path}/pipeline/Data.h
    ${include_path}/pipeline/AbstractInputSlot.h
    
    ${include_path}/plugin/plugin_api.h
    ${include_path}/plugin/PluginManager.h
    ${include_path}/plugin/PluginLibrary.h
    ${include_path}/plugin/Plugin.h
    ${include_path}/plugin/PainterPlugin.h
    ${include_path}/plugin/PainterPlugin.hpp
    
    ${include_path}/primitives/AbstractDrawable.h
    ${include_path}/primitives/UniformGroup.hpp
    ${include_path}/primitives/Interpolation.h
    ${include_path}/primitives/Drawable.h
    ${include_path}/primitives/Drawable.hpp
    ${include_path}/primitives/AdaptiveGrid.h
    ${include_path}/primitives/ScreenAlignedQuad.h
    ${include_path}/primitives/Icosahedron.h
    ${include_path}/primitives/AxisAlignedBoundingBox.h
    ${include_path}/primitives/UniformGroup.h
    ${include_path}/primitives/PolygonalGeometry.h
    ${include_path}/primitives/PolygonalDrawable.h
    ${include_path}/primitives/Scene.h
    ${include_path}/primitives/RenderPass.h
    
    ${include_path}/resources/ResourceManager.hpp
    ${include_path}/resources/RawFile.h
    ${include_path}/resources/AbstractStorer.h
    ${include_path}/resources/ResourceManager.h
    ${include_path}/resources/AbstractLoader.h
    ${include_path}/resources/Loader.hpp
    ${include_path}/resources/Storer.hpp
    ${include_path}/resources/Storer.h
    ${include_path}/resources/GlrawTextureLoader.h
    ${include_path}/resources/Loader.h
    
    ${include_path}/stages/ColorGradientPreparationStage.h
    ${include_path}/stages/ColorGradientSelectionStage.h
    ${include_path}/stages/ColorGradientTextureStage.h
    
    ${include_path}/tools/ImageExporter.h
    ${include_path}/tools/DepthExtractor.h
    ${include_path}/tools/WorldExtractor.h
    ${include_path}/tools/ObjectIdExtractor.h
    ${include_path}/tools/ColorExtractor.h
    ${include_path}/tools/NormalExtractor.h
    ${include_path}/tools/GBufferExtractor.h
    ${include_path}/tools/ColorGradientPreparation.h
)

set(sources
    ${source_path}/gloperate.cpp
    
    ${source_path}/base/AbstractColorGradient.cpp
    ${source_path}/base/ColorGradientList.cpp
    ${source_path}/base/ExplicitEquidistantColorGradient.cpp
    ${source_path}/base/RenderTarget.cpp
    ${source_path}/base/dirent_msvc.h
    ${source_path}/base/registernamedstrings.cpp
    ${source_path}/base/ChronoTimer.cpp
    ${source_path}/base/AutoTimer.cpp
    ${source_path}/base/CyclicTime.cpp
    ${source_path}/base/AbstractWindow.cpp
    
    ${source_path}/input/KeyboardEvent.cpp
    ${source_path}/input/WheelEvent.cpp
    ${source_path}/input/AbstractEvent.cpp
    ${source_path}/input/AbstractEventProvider.cpp
    ${source_path}/input/MouseInputHandler.cpp
    ${source_path}/input/KeyboardInputHandler.cpp
    ${source_path}/input/MouseEvent.cpp
    
    ${source_path}/navigation/AbstractMapping.cpp
    ${source_path}/navigation/AbstractInteraction.cpp
    ${source_path}/navigation/navigationmath.cpp
    ${source_path}/navigation/WorldInHandNavigation.cpp
    ${source_path}/navigation/CoordinateProvider.cpp
    
    ${source_path}/painter/PerspectiveProjectionCapability.cpp
    ${source_path}/painter/TargetFramebufferCapability.cpp
    ${source_path}/painter/CameraCapability.cpp
    ${source_path}/painter/TypedRenderTargetCapability.cpp
    ${source_path}/painter/Painter.cpp
    ${source_path}/painter/AbstractVirtualTimeCapability.cpp
    ${source_path}/painter/ViewportCapability.cpp
    ${source_path}/painter/AbstractInputCapability.cpp
    ${source_path}/painter/AbstractProjectionCapability.cpp
    ${source_path}/painter/Camera.cpp
    ${source_path}/painter/ContextFormat.cpp
    ${source_path}/painter/AbstractCameraCapability.cpp
    ${source_path}/painter/AbstractPerspectiveProjectionCapability.cpp
    ${source_path}/painter/AbstractCapability.cpp
    ${source_path}/painter/InputCapability.cpp
    ${source_path}/painter/AbstractTypedRenderTargetCapability.cpp
    ${source_path}/painter/VirtualTimeCapability.cpp
    ${source_path}/painter/AbstractContext.cpp
    ${source_path}/painter/AbstractTargetFramebufferCapability.cpp
    ${source_path}/painter/AbstractViewportCapability.cpp
    ${source_path}/painter/AbstractMetaInformationCapability.cpp
    ${source_path}/painter/MetaInformationCapability.cpp
    ${source_path}/painter/AbstractOutputCapability.cpp
    ${source_path}/painter/PipelineOutputCapability.cpp
    ${source_path}/painter/AbstractOrthographicProjectionCapability.cpp
    ${source_path}/painter/OrthographicProjectionCapability.cpp
    
    ${source_path}/pipeline/AbstractInputSlot.cpp
    ${source_path}/pipeline/InputSlot.cpp
    ${source_path}/pipeline/PipelinePainter.cpp
    ${source_path}/pipeline/AbstractStage.cpp
    ${source_path}/pipeline/AbstractPipeline.cpp
    ${source_path}/pipeline/AbstractData.cpp
    
    ${source_path}/plugin/PluginManager.cpp
    ${source_path}/plugin/PluginLibrary.cpp
    ${source_path}/plugin/Plugin.cpp
    ${source_path}/plugin/PainterPlugin.cpp

    ${source_path}/primitives/AbstractDrawable.cpp
    ${source_path}/primitives/Plane3.cpp
    ${source_path}/primitives/Drawable.cpp
    ${source_path}/primitives/UniformGroup.cpp
    ${source_path}/primitives/ScreenAlignedQuad.cpp
    ${source_path}/primitives/AxisAlignedBoundingBox.cpp
    ${source_path}/primitives/Icosahedron.cpp
    ${source_path}/primitives/Plane3.h
    ${source_path}/primitives/AdaptiveGrid.cpp
    ${source_path}/primitives/PolygonalGeometry.cpp
    ${source_path}/primitives/PolygonalDrawable.cpp
    ${source_path}/primitives/Scene.cpp
    ${source_path}/primitives/RenderPass.cpp
    
    ${source_path}/resources/AbstractStorer.cpp
    ${source_path}/resources/AbstractLoader.cpp
    ${source_path}/resources/GlrawTextureLoader.cpp
    ${source_path}/resources/RawFile.cpp
    ${source_path}/resources/ResourceManager.cpp
    
    ${source_path}/stages/ColorGradientPreparationStage.cpp
    ${source_path}/stages/ColorGradientSelectionStage.cpp
    ${source_path}/stages/ColorGradientTextureStage.cpp
    
    ${source_path}/tools/ImageExporter.cpp
    ${source_path}/tools/DepthExtractor.cpp
    ${source_path}/tools/WorldExtractor.cpp
    ${source_path}/tools/ObjectIdExtractor.cpp
    ${source_path}/tools/ColorExtractor.cpp
    ${source_path}/tools/NormalExtractor.cpp
    ${source_path}/tools/GBufferExtractor.cpp
    ${source_path}/tools/ColorGradientPreparation.cpp
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
    $<INSTALL_INTERFACE:include>
)


# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE
    ${OPENGL_LIBRARIES}
    cpplocate::cpplocate
    libzeug::reflectionzeug
    libzeug::iozeug
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
