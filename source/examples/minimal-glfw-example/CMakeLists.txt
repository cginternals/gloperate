
# 
# External dependencies
# 

find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(libzeug REQUIRED)
find_package(GLFW)


# 
# Executable name and options
# 

# Target name
set(target minimal-glfw-example)

# Exit here if required dependencies are not met
if (NOT GLFW_FOUND)
    message(STATUS "Example ${target} skipped: GLFW not found")
    return()
else()
    message(STATUS "Example ${target}")
endif()


# 
# Sources
# 

set(sources
    main.cpp
)


# 
# Create executable
# 

# Build executable
add_executable(${target}
    MACOSX_BUNDLE
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
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GLM_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/source/gloperate/include
    ${PROJECT_SOURCE_DIR}/source/gloperate-glfw/include
)


# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    cpplocate::cpplocate
    libzeug::signalzeug
    libzeug::reflectionzeug
    glbinding::glbinding
    globjects::globjects
    ${META_PROJECT_NAME}::gloperate
    ${META_PROJECT_NAME}::gloperate-glfw
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


# 
# Deployment
# 

# Executable
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT runtime
    BUNDLE  DESTINATION ${INSTALL_BIN} COMPONENT runtime
)
