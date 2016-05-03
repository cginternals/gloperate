
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

    ${include_path}/base/CachedValue.h
    ${include_path}/base/CachedValue.hpp
    ${include_path}/base/ChronoTimer.h
    ${include_path}/base/AutoTimer.h
    ${include_path}/base/Image.h
    ${include_path}/base/ResourceManager.h
    ${include_path}/base/ResourceManager.hpp
    ${include_path}/base/AbstractLoader.h
    ${include_path}/base/AbstractStorer.h
    ${include_path}/base/Loader.h
    ${include_path}/base/Loader.hpp
    ${include_path}/base/Storer.h
    ${include_path}/base/Storer.hpp

    ${include_path}/viewer/AbstractContext.h
    ${include_path}/viewer/AbstractGLContext.h
    ${include_path}/viewer/AbstractGLContextFactory.h
    ${include_path}/viewer/GLContextFormat.h
    ${include_path}/viewer/GLContextUtils.h
    ${include_path}/viewer/ViewerContext.h
    ${include_path}/viewer/Surface.h
    ${include_path}/viewer/RenderSurface.h
    ${include_path}/viewer/TimeManager.h
    ${include_path}/viewer/Timer.h

    ${include_path}/scripting/ScriptEnvironment.h
    ${include_path}/scripting/SystemApi.h
    ${include_path}/scripting/TimerApi.h
    ${include_path}/scripting/ScriptTimer.h
    ${include_path}/scripting/ComponentsApi.h

    ${include_path}/pipeline/Stage.h

    ${include_path}/rendering/Camera.h
    
    ${include_path}/stages/demos/DemoStage.h

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
)

set(sources
    ${source_path}/gloperate.cpp

    ${source_path}/base/ChronoTimer.cpp
    ${source_path}/base/AutoTimer.cpp
    ${source_path}/base/Image.cpp
    ${source_path}/base/ResourceManager.cpp
    ${source_path}/base/AbstractLoader.cpp
    ${source_path}/base/AbstractStorer.cpp

    ${source_path}/viewer/AbstractContext.cpp
    ${source_path}/viewer/AbstractGLContext.cpp
    ${source_path}/viewer/AbstractGLContextFactory.cpp
    ${source_path}/viewer/GLContextFormat.cpp
    ${source_path}/viewer/GLContextUtils.cpp
    ${source_path}/viewer/ViewerContext.cpp
    ${source_path}/viewer/Surface.cpp
    ${source_path}/viewer/RenderSurface.cpp
    ${source_path}/viewer/TimeManager.cpp
    ${source_path}/viewer/Timer.cpp

    ${source_path}/scripting/ScriptEnvironment.cpp
    ${source_path}/scripting/SystemApi.cpp
    ${source_path}/scripting/TimerApi.cpp
    ${source_path}/scripting/ScriptTimer.cpp
    ${source_path}/scripting/ComponentsApi.cpp

    ${source_path}/pipeline/Stage.cpp

    ${source_path}/rendering/Camera.cpp

    ${source_path}/stages/demos/DemoStage.cpp

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
