
# Check if examples are enabled
if(NOT OPTION_BUILD_EXAMPLES)
    return()
endif()

# Example plugins
#add_subdirectory(globjects-painters)
#add_subdirectory(gloperate-text-painters)
#add_subdirectory(osg-painters)

# Example applications
add_subdirectory(gloperate-glfw-example)
add_subdirectory(gloperate-qt-example)
add_subdirectory(gloperate-qtquick-example)
#add_subdirectory(gloperate-ffmpeg-example)
#add_subdirectory(gloperate-videotool-example)
add_subdirectory(gloperate-text-example)
