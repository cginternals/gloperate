
#include <gloperate/plugin/plugin_api.h>

#include "osg-scene/OsgScene.h"


GLOPERATE_PLUGIN_LIBRARY

    GLOPERATE_PAINTER_PLUGIN(OsgScene
    , "OsgScene"
    , "Load a simple OSG scene"
    , "gloperate team"
    , "v1.0.0" )

GLOPERATE_PLUGIN_LIBRARY_END
