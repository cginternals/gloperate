
#include <cppexpose/plugin/plugin_api.h>

#include "ColorizeStage.h"
#include "DemoAntialiasableTriangleStage.h"
#include "DemoAntialiasingPipeline.h"
#include "DemoDOFCubeStage.h"
#include "DemoDOFPipeline.h"
#include "DemoAntialiasingAggregationPipeline.h"
#include "DemoDOFAggregationPipeline.h"
#include "DemoSSAOAggregationPipeline.h"
#include "DemoTransparencyAggregationPipeline.h"
#include "DemoPipeline.h"
#include "DemoRenderStage.h"
#include "DemoSSAOPipeline.h"
#include "DemoSSAOPostprocessingStage.h"
#include "DemoSSAORenderingStage.h"
#include "DemoStage.h"
#include "DemoTransparencyPipeline.h"
#include "DemoTransparencyStage.h"
#include "DemoTextRenderingPipeline.h"
#include "GlyphSequenceDemoStage.h"
#include "LightTestPipeline.h"
#include "LightTestStage.h"
#include "ShaderDemoPipeline.h"
#include "SpinningRectStage.h"
#include "TimerStage.h"


CPPEXPOSE_PLUGIN_LIBRARY

    CPPEXPOSE_PLUGIN_COMPONENT(ColorizeStage)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoAntialiasableTriangleStage)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoAntialiasingPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoDOFCubeStage)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoDOFPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoAntialiasingAggregationPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoDOFAggregationPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoSSAOAggregationPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoTransparencyAggregationPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoRenderStage)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoSSAOPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoSSAOPostprocessingStage)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoSSAORenderingStage)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoStage)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoTransparencyPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoTransparencyStage)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoTextRenderingPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(GlyphSequenceDemoStage)
    CPPEXPOSE_PLUGIN_COMPONENT(LightTestPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(LightTestStage)
    CPPEXPOSE_PLUGIN_COMPONENT(ShaderDemoPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(SpinningRectStage)
    CPPEXPOSE_PLUGIN_COMPONENT(TimerStage)

CPPEXPOSE_PLUGIN_LIBRARY_END
