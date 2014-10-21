#include <gloperate/plugin/plugin_api.h>

#include <basic-examples/SimpleTexture/SimpleTexture.h>
#include <basic-examples/RotatingQuad/RotatingQuad.h>
#include <basic-examples/CubeScape/CubeScape.h>

#include <gloperate-version.h>

GLOPERATE_PLUGIN_LIBRARY

    GLOPERATE_PLUGIN(SimpleTexture
    , "SimpleTexture"
    , "Renders a simple generated texture"
    , GLOPERATE_AUTHOR_ORGANIZATION
    , "v1.0.0" )

    GLOPERATE_PLUGIN(RotatingQuad
    , "RotatingQuad"
    , "Renders a rotating quad, featuring animation and cameras"
    , GLOPERATE_AUTHOR_ORGANIZATION
    , "v1.0.0" )

    GLOPERATE_PLUGIN(CubeScape
    , "CubeScape"
    , "Renders a landscape built of cubes"
    , GLOPERATE_AUTHOR_ORGANIZATION
    , "v1.0.0" )

GLOPERATE_PLUGIN_LIBRARY_END
