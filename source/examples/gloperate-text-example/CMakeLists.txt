
# 
# External dependencies
# 

find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(cppexpose REQUIRED)
find_package(cppassist REQUIRED)
find_package(Qt5Core    5.1)
find_package(Qt5Gui     5.1)
find_package(Qt5Widgets 5.1)
find_package(Qt5Quick   5.1)

# Enable automoc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)
set_property(GLOBAL PROPERTY AUTOMOC_FOLDER CMakeAutomocTargets)

# ENABLE CMP0020: Automatically link Qt executables to qtmain target on Windows.
cmake_policy(SET CMP0020 NEW)


# 
# Executable name and options
# 

# Target name
set(target gloperate-text-example)

# Exit here if required dependencies are not met
if (NOT Qt5Core_FOUND)
    message(STATUS "Example ${target} skipped: Qt5 not found")
    return()
else()
    message(STATUS "Example ${target}")
endif()


# 
# Sources
# 

set(sources
    main.cpp
    GlyphSequenceDemoStage.cpp
    GlyphSequenceDemoStage.h
    TextRenderingPipeline.cpp
    TextRenderingPipeline.h
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
    ${PROJECT_SOURCE_DIR}/source/gloperate-qt/include
    ${PROJECT_SOURCE_DIR}/source/gloperate-qtquick/include
    ${PROJECT_SOURCE_DIR}/source/gloperate-text/include
)


# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    cpplocate::cpplocate
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Quick
    cppexpose::cppexpose
    cppassist::cppassist
    glbinding::glbinding
    globjects::globjects
    ${META_PROJECT_NAME}::gloperate
    ${META_PROJECT_NAME}::gloperate-qt
    ${META_PROJECT_NAME}::gloperate-qtquick
    ${META_PROJECT_NAME}::gloperate-text
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
