if(OPTION_BUILD_EXAMPLES)

    # Example plugins
    add_subdirectory(basic-painters)
    add_subdirectory(extended-painters)
    add_subdirectory(PipelineExamples)
    add_subdirectory(OsgExamples)

    # Example applications
    add_subdirectory(ExtendedViewerQt)
    add_subdirectory(viewer-glfw)
    add_subdirectory(viewer-qt)

endif()
