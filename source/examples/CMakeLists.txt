
# Check if examples are enabled
if(NOT OPTION_BUILD_EXAMPLES)
    return()
endif()

# Example plugins
add_subdirectory(demo-stages-plugins)

# Example applications
add_subdirectory(gloperate-glfw-example)
add_subdirectory(gloperate-qt-example)
add_subdirectory(gloperate-qtquick-example)
add_subdirectory(gloperate-headless-example)
#add_subdirectory(gloperate-ffmpeg-example)
#add_subdirectory(gloperate-videotool-example)
