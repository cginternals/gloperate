
# 
# Configuration for all sub-projects
# 

# Generate version-header
configure_file(version.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME}/${META_PROJECT_NAME}-version.h)


# 
# Sub-projects
# 

# Libraries
set(IDE_FOLDER "")
add_subdirectory(gloperate)
add_subdirectory(gloperate-glfw)
add_subdirectory(gloperate-qt)
add_subdirectory(gloperate-qtquick)
add_subdirectory(gloperate-hidapi)
add_subdirectory(gloperate-glkernel)
#add_subdirectory(gloperate-ffmpeg)
#add_subdirectory(gloperate-assimp)
#add_subdirectory(gloperate-osg)
add_subdirectory(gloperate-text)

# Plugins
set(IDE_FOLDER "Plugins")
add_subdirectory(plugins)

# Applications
set(IDE_FOLDER "Applications")
add_subdirectory(applications)

# Tools
#set(IDE_FOLDER "Tools")
#add_subdirectory(tools)

# Examples
set(IDE_FOLDER "Examples")
add_subdirectory(examples)

# Tests
#if(OPTION_BUILD_TESTS)
#    set(IDE_FOLDER "Tests")
#    add_subdirectory(tests)
#endif()


# 
# Deployment
# 

# Deploy generated headers
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME} DESTINATION include COMPONENT dev)
