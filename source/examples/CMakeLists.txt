
if(OPTION_BUILD_EXAMPLES)
    
    # Example plugins
    add_subdirectory(globjects-painters)
    add_subdirectory(osg-painters)

    # Example applications
    add_subdirectory(minimal-glfw-example)
    add_subdirectory(minimal-qt-example)

endif()
