#include <gloperate/plugin/plugin_api.h>
#include <basic-examples/SimpleTexture/SimpleTexture.h>
#include <basic-examples/RotatingQuad/RotatingQuad.h>
#include <basic-examples/CubeScape/CubeScape.h>


GLOPERATE_PLUGIN_LIBRARY

    GLOPERATE_PLUGIN(SimpleTexture
    , "SimpleTexture"
    , "Displays a simple generated texture on the screen"
    , "gloperate team"
    , "v1.0.0" )

    GLOPERATE_PLUGIN(RotatingQuad
    , "RotatingQuad"
    , "Displays a rotating quad geometry, demonstrating animation and cameras"
    , "gloperate team"
    , "v1.0.0" )

    GLOPERATE_PLUGIN(CubeScape
    , "CubeScape"
    , "Displays a CubeScape"
    , "gloperate team"
    , "v1.0.0" )

GLOPERATE_PLUGIN_LIBRARY_END
