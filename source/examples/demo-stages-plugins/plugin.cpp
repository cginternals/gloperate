
#include <cppexpose/plugin/plugin_api.h>

#include "AssimpMeshLoader.h"
#include "ColorGradientDemo.h"
#include "DemoDrawableStage.h"
#include "DemoMultiFrameAggregationPipeline.h"
#include "DemoTextRenderingPipeline.h"
#include "DemoTransparencyAggregationPipeline.h"
#include "GlyphSequenceDemoStage.h"
#include "LightTestPipeline.h"
#include "LightTestStage.h"
#include "MultiFramePostprocessingStage.h"
#include "MultiFrameRenderingPipeline.h"
#include "MultiFrameSceneRenderingStage.h"
#include "ShaderDemoPipeline.h"
#include "ShapeDemo.h"
#include "TransparencyRenderingPipeline.h"
#include "TransparentCirclesStage.h"


CPPEXPOSE_PLUGIN_LIBRARY

    CPPEXPOSE_PLUGIN_COMPONENT(AssimpMeshLoader)
    CPPEXPOSE_PLUGIN_COMPONENT(ColorGradientDemo)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoDrawableStage)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoMultiFrameAggregationPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoTextRenderingPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoTransparencyAggregationPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(GlyphSequenceDemoStage)
    CPPEXPOSE_PLUGIN_COMPONENT(LightTestPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(LightTestStage)
    CPPEXPOSE_PLUGIN_COMPONENT(MultiFramePostprocessingStage)
    CPPEXPOSE_PLUGIN_COMPONENT(MultiFrameRenderingPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(MultiFrameSceneRenderingStage)
    CPPEXPOSE_PLUGIN_COMPONENT(ShaderDemoPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(ShapeDemo)
    CPPEXPOSE_PLUGIN_COMPONENT(TransparencyRenderingPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(TransparentCirclesStage)

CPPEXPOSE_PLUGIN_LIBRARY_END
