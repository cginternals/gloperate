
# Check if tools are enabled
if(NOT OPTION_BUILD_TOOLS)
    return()
endif()

# Tools
add_subdirectory(gloperate-shader-compiler)
