
# Target

set(target gloperate-qtapplication)


# External packages

find_package(Qt5Core    5.1 QUIET)
find_package(Qt5Gui     5.1 QUIET)
find_package(Qt5Widgets 5.1 QUIET) # this is also important in order to get uic working 
find_package(Qt5OpenGL  5.1 QUIET)
find_package(ASSIMP QUIET)

if (NOT Qt5Core_FOUND)
    message(STATUS "Lib ${target} (disabled because Qt5 couldn't be found)")
    return()
endif()

if (ASSIMP_FOUND)
   add_definitions("-DGLOPERATE_ASSIMP_FOUND")
endif()

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)

find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(libzeug REQUIRED)

# http://qt-project.org/forums/viewthread/30006/
if(MSVC)
    cmake_policy(SET CMP0020 NEW)
endif()

set(CMAKE_AUTOMOC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)

# Probably works in the next cmake release -> http://www.cmake.org/Bug/view.php?id=13788
# What we do not want is automocs beside the project -> http://www.cmake.org/Bug/view.php?id=13688
set_property(GLOBAL PROPERTY AUTOMOC_FOLDER CMakeAutomocTargets)


# Announce target
message(STATUS "Lib ${target}")


# Includes

include_directories(
    ${GLM_INCLUDE_DIR}
    ${GLBINDING_INCLUDE_DIRS}
    ${GLOBJECTS_INCLUDE_DIRS}
    ${LIBZEUG_INCLUDE_DIRS}
)

include_directories(
    BEFORE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/source/gloperate/include
    ${CMAKE_SOURCE_DIR}/source/gloperate-qt/include
    ${CMAKE_SOURCE_DIR}/source/gloperate-qtwidgets/include
    ${CMAKE_SOURCE_DIR}/source/gloperate-assimp/include
)


# Libraries

set(libs
    ${GLBINDING_LIBRARIES}
    ${GLOBJECTS_LIBRARIES}
    ${LIBZEUG_LIBRARIES}
    gloperate
    gloperate-qt
    gloperate-qtwidgets
)

if (ASSIMP_FOUND)
    list(APPEND libs "gloperate-assimp")
endif ()


# Compiler definitions

if (OPTION_BUILD_STATIC)
    add_definitions("-DGLOPERATE_STATIC")
else()
    add_definitions("-DGLOPERATE_QTAPPLICATION_EXPORTS")
endif()


# Sources

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(api_includes
    ${include_path}/gloperate-qtapplication_api.h
    ${include_path}/AbstractApplication.h
    ${include_path}/Mapping.h
    ${include_path}/Viewer.h
)

set(sources
    ${source_path}/AbstractApplication.cpp
    ${source_path}/Mapping.cpp
    ${source_path}/Viewer.cpp
    ${source_path}/Viewer.ui
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})


# Build library

list_extract(uis "\\\\.ui$" ${sources})
qt5_wrap_ui(uihs ${uis})

    add_library(${target} ${api_includes} ${sources})
qt5_use_modules(${target} Core Gui Widgets)
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
