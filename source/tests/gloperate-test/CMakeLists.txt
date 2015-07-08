
set(target gloperate-test)
message(STATUS "Test ${target}")


# External libraries

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(libzeug REQUIRED)

# Includes

include_directories(   
    BEFORE
    ${GLM_INCLUDE_DIR}
    ${GLBINDING_INCLUDE_DIRS}
    ${GLOBJECTS_INCLUDE_DIRS}
    ${LIBZEUG_INCLUDE_DIRS}
)

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/source/gloperate/include
)

# Libraries

set(libs
    ${GMOCK_LIBRARIES}
    ${GTEST_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLBINDING_LIBRARIES}
    ${GLOBJECTS_LIBRARIES}
    ${LIBZEUG_LIBRARIES}
    gloperate
)

# Sources

set(sources
    main.cpp
    dummy_test.cpp
    AbstractPipeline_test.cpp
    AbstractStage_test.cpp
    DummyStage.hpp
)

# Build executable

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
