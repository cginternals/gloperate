message(STATUS "Configuring for platform Linux/GCC.")


# Enable C++11 support

execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
    OUTPUT_VARIABLE GCC_VERSION)

if(GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
elseif(GCC_VERSION VERSION_EQUAL 4.6)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()


set(LINUX_COMPILE_DEFS
    LINUX                      # Linux system
    PIC                       # Position-independent code
    _REENTRANT                # Reentrant code
)
set(DEFAULT_COMPILE_DEFS_DEBUG
    ${LINUX_COMPILE_DEFS}
    _DEBUG                      # Debug build
)
set(DEFAULT_COMPILE_DEFS_RELEASE
    ${LINUX_COMPILE_DEFS}
    NDEBUG                      # Release build
)

set(LINUX_COMPILE_FLAGS
      
      -fno-exceptions
      -pthread      # -> use pthread library
      -pipe         # -> use pipes
      -fPIC         # -> use position independent code
    # -no-rtti      # -> disable c++ rtti

      -Wall         # -> 
      -pedantic     # ->
      -Wextra       # -> 
      -Werror       # ->
      
      -Wreturn-type 
      -Wno-cast-qual 
      -Wcast-align 
      -Wno-conversion 
      -Wno-error=comment
      -Wno-error=strict-aliasing

      -Wno-float-equal 
      -Wno-pragmas

    # -Werror=return-type -> missing returns in functions and methods are handled as errors which stops the compilation
    # -Wshadow      # -> e.g. when a parameter is named like a member, too many warnings, disabled for now

      -fvisibility=hidden
)

if (CMAKE_COMPILER_IS_GNUCXX)
    if (NOT (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "4.9"))
        set(LINUX_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS}
            -Wno-float-conversion
        )
    endif ()
endif ()


set(DEFAULT_COMPILE_FLAGS
    ${LINUX_COMPILE_FLAGS}
    $<$<CONFIG:Debug>:   
    >
    $<$<CONFIG:Release>: 
    >
)

set(LINUX_LINKER_FLAGS "-pthread")

set(DEFAULT_LINKER_FLAGS_RELEASE ${LINUX_LINKER_FLAGS})
set(DEFAULT_LINKER_FLAGS_DEBUG ${LINUX_LINKER_FLAGS})
set(DEFAULT_LINKER_FLAGS ${LINUX_LINKER_FLAGS})

# Add platform specific libraries for linking
set(EXTRA_LIBS "")
