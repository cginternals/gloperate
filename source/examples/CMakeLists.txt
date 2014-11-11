if(OPTION_BUILD_EXAMPLES)
	
    # Example plugins
    add_subdirectory(basic-painters)
    add_subdirectory(pipeline-painters)
    add_subdirectory(extended-painters)
    add_subdirectory(osg-painters)

    # Example applications
    add_subdirectory(viewer-glfw)
    add_subdirectory(viewer-qt)
    add_subdirectory(viewer-qt-extended)

endif()
