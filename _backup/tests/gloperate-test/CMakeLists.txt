
# 
# External dependencies
# 

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(libzeug REQUIRED)


# 
# Executable name and options
# 

# Target name
set(target gloperate-test)
message(STATUS "Test ${target}")


# 
# Sources
# 

set(sources
    main.cpp
    dummy_test.cpp
    AbstractPipeline_test.cpp
    AbstractStage_test.cpp
    DummyStage.hpp
)


# 
# Create executable
# 

# Build executable
add_executable(${target}
    ${sources}
)

# Create namespaced alias
add_executable(${META_PROJECT_NAME}::${target} ALIAS ${target})


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
    ${GLM_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/source/gloperate/include
    ${PROJECT_BINARY_DIR}/source/include
)


# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    ${OPENGL_LIBRARIES}
    libzeug::signalzeug
    glbinding::glbinding
    globjects::globjects
    ${META_PROJECT_NAME}::gloperate
    gmock-dev
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LINKER_OPTIONS}
)
