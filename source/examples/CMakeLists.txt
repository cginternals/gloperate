if(OPTION_BUILD_EXAMPLES)
	
    # Example plugins
    add_subdirectory(globjects-painters)
    add_subdirectory(pipeline-painters)
    add_subdirectory(osg-painters)

    # Example applications
    add_subdirectory(viewer-glfw)
    add_subdirectory(viewer-qt)

endif()
