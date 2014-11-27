#include <gloperate/plugin/plugin_api.h>

#include "postprocessing/Postprocessing.h"


GLOPERATE_PLUGIN_LIBRARY

    GLOPERATE_PLUGIN(Postprocessing
    , "PostprocessingPipeline"
    , "Displays a postprocessed Icosahedron using a pipeline"
    , "gloperate team"
    , "v1.0.0" )

GLOPERATE_PLUGIN_LIBRARY_END
