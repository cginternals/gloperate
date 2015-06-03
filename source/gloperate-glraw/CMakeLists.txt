
# Target
set(target gloperate-glraw)


#
# External libraries
#

find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(libzeug REQUIRED)
find_package(glraw QUIET)

# Build only if GLRAW is available
if (NOT GLRAW_INCLUDE_DIR)
    message("Library ${target} skipped: GLRAW not found")
    return()
endif()


#
# Qt5
#

# good resource: http://www.kdab.com/using-cmake-with-qt-5/

# http://qt-project.org/forums/viewthread/30006/
if(MSVC)
    cmake_policy(SET CMP0020 NEW)
endif()

find_package(Qt5OpenGL  5.1 QUIET)
find_package(Qt5Core    5.1 QUIET)
find_package(Qt5Gui     5.1 QUIET)
find_package(Qt5Widgets 5.1 QUIET)

if (NOT Qt5Core_FOUND)
    message(STATUS "Example ${target} (disabled because Qt5 couldn't be found)")
    return()
endif()


set(CMAKE_AUTOMOC ON)

message(STATUS "Lib ${target}")


#
# Includes
#

include_directories(
    ${GLM_INCLUDE_DIR}
    ${GLBINDING_INCLUDES}
    ${GLOBJECTS_INCLUDES}
    ${LIBZEUG_INCLUDES}
    ${GLRAW_INCLUDES}
)

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/source/gloperate/include
    ${CMAKE_SOURCE_DIR}/source/gloperate-qt/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


#
# Libraries
#

set(libs
    ${GLBINDING_LIBRARIES}
    ${GLOBJECTS_LIBRARIES}
    ${LIBZEUG_LIBRARIES}
    ${GLRAW_LIBRARIES}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::OpenGL
    gloperate
    gloperate-qt
)


#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DGLOPERATE_STATIC")
else()
    add_definitions("-DGLOPERATE_GLRAW_EXPORTS")
endif()

# for compatibility between glm 0.9.4 and 0.9.5
add_definitions("-DGLM_FORCE_RADIANS")


#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(sources
    ${source_path}/TextureFilter.cpp
    ${source_path}/glraw.cpp
)

set(headers
    ${include_path}/gloperate-glraw_api.h
    ${include_path}/TextureFilter.h
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
