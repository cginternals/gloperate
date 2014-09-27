#include <gloperate/plugin/plugin_api.h>

#include <extended-examples/ExtendedCubeScape/ExtendedCubeScape.h>
#include <gloperate-version.h>


GLOPERATE_PLUGIN_LIBRARY

    GLOPERATE_PLUGIN(ExtendedCubeScape
    , "ExtendedCubeScape"
    , "Displays a CubeScape"
    , GLOPERATE_AUTHOR_ORGANIZATION
    , "v1.0.0" )

GLOPERATE_PLUGIN_LIBRARY_END
