# Target
set(target gloperate)
message(STATUS "Lib ${target}")

#
# External libraries
#

find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)

#
# Includes
#

include_directories(
    ${GLM_INCLUDE_DIR}
    ${GLBINDING_INCLUDES}
    ${GLOBJECTS_INCLUDES}
)

include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#
# Libraries
#

set(libs
    ${GLBINDING_LIBRARIES}
    ${GLOBJECTS_LIBRARIES}
)

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DGLOPERATE_STATIC")
else()
    add_definitions("-DGLOPERATE_EXPORTS")
endif()

# for compatibility between glm 0.9.4 and 0.9.5
add_definitions("-DGLM_FORCE_RADIANS")

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(sources
	${source_path}/base/ChronoTimer.cpp
	${source_path}/base/AutoTimer.cpp
	${source_path}/base/CyclicTime.cpp
	${source_path}/Camera.cpp
	${source_path}/capabilities/TargetFramebufferCapability.cpp
	${source_path}/capabilities/AbstractVirtualTimeCapability.cpp
	${source_path}/capabilities/ViewportCapability.cpp
	${source_path}/capabilities/AbstractCapability.cpp
	${source_path}/capabilities/VirtualTimeCapability.cpp
	${source_path}/capabilities/AbstractTargetFramebufferCapability.cpp
	${source_path}/capabilities/AbstractViewportCapability.cpp
	${source_path}/Painter.cpp
	${source_path}/pipelines/AbstractInputSlot.cpp
	${source_path}/pipelines/InputSlot.cpp
	${source_path}/pipelines/PipelinePainter.cpp
	${source_path}/pipelines/AbstractStage.cpp
	${source_path}/pipelines/Nameable.cpp
	${source_path}/pipelines/StageOwned.cpp
	${source_path}/pipelines/AbstractPipeline.cpp
	${source_path}/pipelines/AbstractParameter.cpp
	${source_path}/pipelines/AbstractData.cpp
	${source_path}/plugin/PluginManager.cpp
	${source_path}/plugin/Plugin.cpp
	${source_path}/plugin/PluginLibrary.cpp
	${source_path}/resources/AbstractStorer.cpp
	${source_path}/resources/AbstractLoader.cpp
	${source_path}/resources/GlrawTextureLoader.cpp
	${source_path}/resources/TextureStorer.cpp
	${source_path}/resources/TextureLoader.cpp
	${source_path}/resources/RawFile.cpp
	${source_path}/resources/ResourceManager.cpp
	${source_path}/signals/AbstractSignal.cpp
	${source_path}/signals/Connection.cpp
	${source_path}/signals/ScopedConnection.cpp
	${source_path}/signals/ConnectionMap.cpp
	${source_path}/tools/ScreenshotTool.cpp
	${source_path}/util/StringTemplate.cpp
	${source_path}/util/ScreenAlignedQuad.cpp
	${source_path}/util/AxisAlignedBoundingBox.cpp
	${source_path}/util/StringSourceDecorator.cpp
)

set(headers
	${include_path}/base/CyclicTime.h
	${include_path}/base/ChronoTimer.h
	${include_path}/base/AutoTimer.h
	${include_path}/Camera.h
	${include_path}/capabilities/AbstractViewportCapability.h
	${include_path}/capabilities/AbstractTargetFramebufferCapability.h
	${include_path}/capabilities/AbstractCapability.h
	${include_path}/capabilities/ViewportCapability.h
	${include_path}/capabilities/VirtualTimeCapability.h
	${include_path}/capabilities/TargetFramebufferCapability.h
	${include_path}/capabilities/AbstractVirtualTimeCapability.h
	${include_path}/gloperate_api.h
	${include_path}/Painter.h
	${include_path}/Painter.hpp
	${include_path}/pipelines/PerformanceMonitor.h
	${include_path}/pipelines/Parameter.hpp
	${include_path}/pipelines/StageOwned.h
	${include_path}/pipelines/AbstractParameter.h
	${include_path}/pipelines/AbstractData.h
	${include_path}/pipelines/AbstractPipeline.hpp
	${include_path}/pipelines/Parameter.h
	${include_path}/pipelines/Data.hpp
	${include_path}/pipelines/AbstractStage.h
	${include_path}/pipelines/Nameable.h
	${include_path}/pipelines/InputSlot.h
	${include_path}/pipelines/PipelinePainter.h
	${include_path}/pipelines/InputSlot.hpp
	${include_path}/pipelines/AbstractPipeline.h
	${include_path}/pipelines/Data.h
	${include_path}/pipelines/AbstractInputSlot.h
	${include_path}/plugin/plugin_api.h
	${include_path}/plugin/Plugin.h
	${include_path}/plugin/PainterPlugin.hpp
	${include_path}/plugin/PluginLibrary.h
	${include_path}/plugin/PainterPlugin.h
	${include_path}/plugin/PluginManager.h
	${include_path}/resources/TextureLoader.h
	${include_path}/resources/RawFile.h
	${include_path}/resources/AbstractStorer.h
	${include_path}/resources/ResourceManager.h
	${include_path}/resources/AbstractLoader.h
	${include_path}/resources/GlrawTextureLoader.h
	${include_path}/resources/TextureStorer.h
	${include_path}/signals/ScopedConnection.h
	${include_path}/signals/Signal.h
	${include_path}/signals/Signal.hpp
	${include_path}/signals/Connection.h
	${include_path}/signals/ConnectionMap.hpp
	${include_path}/signals/ConnectionMap.h
	${include_path}/signals/signalzeug_api.h
	${include_path}/signals/AbstractSignal.h
	${include_path}/tools/ScreenshotTool.h
	${include_path}/util/StringTemplate.h
	${include_path}/util/StringSourceDecorator.h
	${include_path}/util/collection.hpp
	${include_path}/util/ScreenAlignedQuad.h
	${include_path}/util/AxisAlignedBoundingBox.h
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${headers})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})

#
# Build library
#

add_library(${target} ${headers} ${sources})

target_link_libraries(${target} ${libs})

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      "${IDE_FOLDER}"
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH                ${include_path})

#
# Deployment
#

# Library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

# Header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE})
