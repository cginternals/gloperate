if(OPTION_BUILD_EXAMPLES)

    # Example plugins
    add_subdirectory(BasicExamples)
    add_subdirectory(ExtendedExamples)
    add_subdirectory(OsgExamples)

    # Example applications
    add_subdirectory(ExtendedViewerQt)
    add_subdirectory(viewer-glfw)
    add_subdirectory(ViewerQt)

endif()
