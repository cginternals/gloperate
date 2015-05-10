
set(target gloperate)
message(STATUS "Lib ${target}")


# External libraries

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(libzeug REQUIRED)

# Includes

include_directories(
    ${GLM_INCLUDE_DIR}
    ${GLBINDING_INCLUDES}
    ${GLOBJECTS_INCLUDES}
    ${LIBZEUG_INCLUDES}
)

include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)


# Libraries

set(libs
    ${OPENGL_LIBRARIES}
    ${GLBINDING_LIBRARIES}
    ${GLOBJECTS_LIBRARIES}
    ${LIBZEUG_LIBRARIES}
)


# Compiler definitions

if (OPTION_BUILD_STATIC)
    add_definitions("-DGLOPERATE_STATIC")
else()
    add_definitions("-DGLOPERATE_EXPORTS")
endif()

# for compatibility between glm 0.9.4 and 0.9.5
add_definitions("-DGLM_FORCE_RADIANS")


# Sources

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(sources
    ${source_path}/base/RenderTarget.cpp
    ${source_path}/base/dirent_msvc.h
    ${source_path}/base/DirectoryIterator.cpp
    ${source_path}/base/directorytraversal.cpp
    ${source_path}/base/DirectoryIterator.h
    ${source_path}/base/ChronoTimer.cpp
    ${source_path}/base/AutoTimer.cpp
    ${source_path}/base/CyclicTime.cpp
    
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
    ${source_path}/plugin/Plugin.cpp
    ${source_path}/plugin/PluginLibrary.cpp

    ${source_path}/primitives/AbstractDrawable.cpp
    ${source_path}/primitives/Plane3.cpp
    ${source_path}/primitives/VertexDrawable.cpp
    ${source_path}/primitives/UniformGroup.cpp
    ${source_path}/primitives/ScreenAlignedQuad.cpp
    ${source_path}/primitives/AxisAlignedBoundingBox.cpp
    ${source_path}/primitives/Icosahedron.cpp
    ${source_path}/primitives/Plane3.h
    ${source_path}/primitives/AdaptiveGrid.cpp
    ${source_path}/primitives/PolygonalGeometry.cpp
    ${source_path}/primitives/PolygonalDrawable.cpp
    ${source_path}/primitives/Scene.cpp
    
    ${source_path}/resources/AbstractStorer.cpp
    ${source_path}/resources/AbstractLoader.cpp
    ${source_path}/resources/GlrawTextureLoader.cpp
    ${source_path}/resources/RawFile.cpp
    ${source_path}/resources/ResourceManager.cpp
    
    ${source_path}/tools/CoordinateProvider.cpp
    ${source_path}/tools/ScreenshotTool.cpp
    ${source_path}/tools/DepthExtractor.cpp
    ${source_path}/tools/WorldExtractor.cpp
    ${source_path}/tools/ObjectIdExtractor.cpp
    ${source_path}/tools/ColorExtractor.cpp
    ${source_path}/tools/NormalExtractor.cpp
    ${source_path}/tools/GBufferExtractor.cpp
)

set(api_includes
    ${include_path}/base/RenderTarget.h
    ${include_path}/base/directorytraversal.h
    ${include_path}/base/collection.hpp
    ${include_path}/base/CyclicTime.h
    ${include_path}/base/ChronoTimer.h
    ${include_path}/base/AutoTimer.h
    ${include_path}/base/make_unique.hpp
    
    ${include_path}/gloperate_api.h
    
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
    ${include_path}/plugin/Plugin.h
    ${include_path}/plugin/PainterPlugin.hpp
    ${include_path}/plugin/PluginLibrary.h
    ${include_path}/plugin/PainterPlugin.h
    ${include_path}/plugin/PluginManager.h
    
    ${include_path}/primitives/AbstractDrawable.h
    ${include_path}/primitives/UniformGroup.hpp
    ${include_path}/primitives/Interpolation.h
    ${include_path}/primitives/VertexDrawable.h
    ${include_path}/primitives/AdaptiveGrid.h
    ${include_path}/primitives/ScreenAlignedQuad.h
    ${include_path}/primitives/Icosahedron.h
    ${include_path}/primitives/AxisAlignedBoundingBox.h
    ${include_path}/primitives/UniformGroup.h
    ${include_path}/primitives/PolygonalGeometry.h
    ${include_path}/primitives/PolygonalDrawable.h
    ${include_path}/primitives/Scene.h
    
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
    
    ${include_path}/tools/CoordinateProvider.h
    ${include_path}/tools/ScreenshotTool.h
    ${include_path}/tools/DepthExtractor.h
    ${include_path}/tools/WorldExtractor.h
    ${include_path}/tools/ObjectIdExtractor.h
    ${include_path}/tools/ColorExtractor.h
    ${include_path}/tools/NormalExtractor.h
    ${include_path}/tools/GBufferExtractor.h
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})


# Build library

add_library(${target} ${api_includes} ${sources})

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
    INCLUDE_PATH                ${include_path})


# Deployment

# Library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

# Header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE})
