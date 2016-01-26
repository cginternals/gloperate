
# Check if examples are enabled
if(NOT OPTION_BUILD_EXAMPLES)
    return()
endif()

# Example plugins
add_subdirectory(globjects-painters)
add_subdirectory(gloperate-text-painters)
add_subdirectory(osg-painters)

# Example applications
add_subdirectory(minimal-glfw-example)
add_subdirectory(minimal-qt-example)
