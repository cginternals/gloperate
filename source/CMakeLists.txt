
# 
# Configuration for all sub-projects
# 

# External packages
find_package(cpplocate REQUIRED)

# Generate version-header
string(TOUPPER ${META_PROJECT_NAME} META_PROJECT_NAME_UPPER)
configure_file(version.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME}/${META_PROJECT_NAME}-version.h)

# Generate module information file
generate_module_info(${META_PROJECT_NAME}
    VALUES
    name        "${META_PROJECT_NAME}"
    version     "${META_VERSION}"
    description "${META_PROJECT_DESCRIPTION}"
    author      "${META_AUTHOR_ORGANIZATION}"

    BUILD_VALUES
    dataPath    "${PROJECT_SOURCE_DIR}/data"

    INSTALL_VALUES
    dataPath    "\${ModulePath}/data"
)

# Create modinfo file in build directory
export_module_info(${META_PROJECT_NAME} TARGET ${META_PROJECT_NAME} FOLDER "cmake")

# Install modinfo file to target path
install_module_info(${META_PROJECT_NAME} DESTINATION ${INSTALL_ROOT} COMPONENT dev)


# 
# Sub-projects
# 

# Libraries
set(IDE_FOLDER "")
add_subdirectory(gloperate)
add_subdirectory(gloperate-glfw)
add_subdirectory(gloperate-qt)
add_subdirectory(gloperate-assimp)
add_subdirectory(gloperate-osg)

# Applications
add_subdirectory(gloperate-viewer)

# Tools
set(IDE_FOLDER "Tools")
add_subdirectory(tools)

# Examples
set(IDE_FOLDER "Examples")
add_subdirectory(examples)

# Tests
set(IDE_FOLDER "Tests")
add_subdirectory(tests)


# 
# Deployment
# 

# Deploy generated headers
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME} DESTINATION include COMPONENT dev)
