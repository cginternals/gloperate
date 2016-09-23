
# 
# External dependencies
# 

find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(cppexpose REQUIRED)
find_package(cppassist REQUIRED)
find_package(glheadless)


# 
# Executable name and options
# 

# Target name
set(target gloperate-glheadless-example)

# Exit here if required dependencies are not met
if (NOT glheadless_FOUND)
    message(STATUS "Example ${target} skipped: glheadless not found")
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
)


# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    cpplocate::cpplocate
    cppexpose::cppexpose
    cppassist::cppassist
    glbinding::glbinding
    globjects::globjects
    glheadless::glheadless
    ${META_PROJECT_NAME}::gloperate
    ${META_PROJECT_NAME}::gloperate-glheadless
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
