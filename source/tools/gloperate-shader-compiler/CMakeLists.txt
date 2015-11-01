
# Target

set(target gloperate-shader-compiler)


# External packages

find_package(Qt5Core    5.2 QUIET)
find_package(Qt5Gui     5.2 QUIET)
find_package(Qt5Widgets 5.2 QUIET)
find_package(Qt5OpenGL  5.2 QUIET)

if (NOT Qt5Core_FOUND)
    message(STATUS "Tool ${target} (disabled because Qt5 couldn't be found)")
    return()
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

message(STATUS "Tool ${target}")


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
    ${CMAKE_SOURCE_DIR}/source/gloperate/include
    ${CMAKE_SOURCE_DIR}/source/gloperate-qt/include
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)


# Libraries

set(libs
    ${GLBINDING_LIBRARIES}
    ${GLOBJECTS_LIBRARIES}
    ${LIBZEUG_LIBRARIES}
    gloperate
    gloperate-qt
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::OpenGL
)


# Sources

set(sources
    main.cpp
    JsonParseError.cpp
    JsonParseError.h
    JsonParseErrorLog.cpp
    JsonParseErrorLog.h
    OpenGLContext.cpp
    OpenGLContext.h
    ShaderCompiler.cpp
    ShaderCompiler.h
)


# Build executable

list_extract(uis "\\\\.ui$" ${sources})
qt5_wrap_ui(uihs ${uis})

add_executable(${target} ${sources})

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
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}")


# Deployment

install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
#   LIBRARY DESTINATION ${INSTALL_SHARED}
#   ARCHIVE DESTINATION ${INSTALL_LIB}
)
