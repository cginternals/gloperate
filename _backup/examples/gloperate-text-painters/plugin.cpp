
#include <gloperate/gloperate-version.h>
#include <gloperate/plugin/plugin_api.h>

#include "textrendering/TextRendering.h"
#include "textrenderingpipeline/TextRenderingPainter.h"


GLOPERATE_PLUGIN_LIBRARY

    GLOPERATE_PAINTER_PLUGIN(TextRendering
    , "TextRendering"
    , "Text rendering demo"
    , GLOPERATE_AUTHOR_ORGANIZATION
    , "v1.0.0" )
    
    GLOPERATE_PAINTER_PLUGIN(TextRenderingPainter
    , "TextRenderingPipeline"
    , "Text rendering demo"
    , GLOPERATE_AUTHOR_ORGANIZATION
    , "v1.0.0" )

GLOPERATE_PLUGIN_LIBRARY_END
