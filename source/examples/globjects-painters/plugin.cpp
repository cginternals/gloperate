#include <gloperate/plugin/plugin_api.h>

#include "textured-quad/TexturedQuad.h"
#include "rotating-quad/RotatingQuad.h"
#include "cubescape/CubeScape.h"

#include <gloperate-version.h>

GLOPERATE_PLUGIN_LIBRARY

    GLOPERATE_PLUGIN(TexturedQuad
    , "TexturedQuad"
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
