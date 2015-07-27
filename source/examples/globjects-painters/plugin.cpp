
#include <gloperate-version.h>
#include <gloperate/plugin/plugin_api.h>

#include "logo/Logo.h"
#include "screenaligned/ScreenAligned.h"
#include "cubescape/CubeScape.h"
#include "postprocessing/Postprocessing.h"


GLOPERATE_PLUGIN_LIBRARY

    GLOPERATE_PAINTER_PLUGIN(Logo
    , "Logo"
    , "Renders a rotating quad, featuring textures, animation, and cameras"
    , GLOPERATE_AUTHOR_ORGANIZATION
    , "v1.0.0" )

    GLOPERATE_PAINTER_PLUGIN(ScreenAligned
    , "ScreenAligned"
    , "Renders a simple generated texture onto the screen"
    , GLOPERATE_AUTHOR_ORGANIZATION
    , "v1.0.0" )

    GLOPERATE_PAINTER_PLUGIN(CubeScape
    , "CubeScape"
    , "Renders a landscape built of cubes"
    , GLOPERATE_AUTHOR_ORGANIZATION
    , "v1.0.0" )

    GLOPERATE_PAINTER_PLUGIN(Postprocessing
    , "PostprocessingPipeline"
    , "Displays a postprocessed Icosahedron using a pipeline"
    , GLOPERATE_AUTHOR_ORGANIZATION
    , "v1.0.0" )

GLOPERATE_PLUGIN_LIBRARY_END
