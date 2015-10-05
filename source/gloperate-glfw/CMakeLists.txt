
# Target
set(target gloperate-glfw)

#
# External libraries
#

find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(libzeug REQUIRED)
find_package(GLFW)

# Build only if glfw is available
if(NOT GLFW_FOUND)
   message(STATUS "Lib ${target} (disabled because GLFW couldn't be found)")
    return()
endif()

message(STATUS "Lib ${target}")

#
# Includes
#

include_directories(
    ${GLM_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLBINDING_INCLUDE_DIRS}
    ${GLOBJECTS_INCLUDE_DIRS}
    ${LIBZEUG_INCLUDE_DIRS}
)

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/source/gloperate/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#
# Libraries
#

set(libs
    ${GLBINDING_LIBRARIES}
    ${GLOBJECTS_LIBRARIES}
    ${LIBZEUG_LIBRARIES}
    ${GLFW_LIBRARY}
    gloperate
)

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DGLOPERATE_STATIC")
else()
    add_definitions("-DGLOPERATE_GLFW_EXPORTS")
endif()

# for compatibility between glm 0.9.4 and 0.9.5
add_definitions("-DGLM_FORCE_RADIANS")

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(sources
    ${source_path}/Application.cpp
    ${source_path}/Context.cpp
    ${source_path}/events.cpp
    ${source_path}/Timer.cpp
    ${source_path}/Window.cpp
    ${source_path}/WindowEventDispatcher.cpp
    ${source_path}/WindowEventHandler.cpp
    ${source_path}/WindowEventHandlerBase.cpp
)

set(headers
    ${include_path}/gloperate-glfw_api.h

    ${include_path}/Application.h
    ${include_path}/Context.h
    ${include_path}/events.h
    ${include_path}/Timer.h
    ${include_path}/Window.h
    ${include_path}/WindowEventDispatcher.h
    ${include_path}/WindowEventHandler.h
    ${include_path}/WindowEventHandlerBase.h
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
