
#include <cppexpose/plugin/plugin_api.h>

#include "ColorGradientDemo.h"
#include "DemoAntialiasableTriangleStage.h"
#include "DemoAntialiasingPipeline.h"
#include "DemoDOFCubeStage.h"
#include "DemoDOFPipeline.h"
#include "DemoMultiFramePipeline.h"
#include "DemoSSAOPipeline.h"
#include "DemoSSAOPostprocessingStage.h"
#include "DemoSSAORenderingStage.h"
#include "DemoTextRenderingPipeline.h"
#include "DemoTransparencyPipeline.h"
#include "DemoTransparencyStage.h"
#include "GlyphSequenceDemoStage.h"
#include "LightTestPipeline.h"
#include "LightTestStage.h"
#include "ShaderDemoPipeline.h"
#include "ShapeDemo.h"


CPPEXPOSE_PLUGIN_LIBRARY

    CPPEXPOSE_PLUGIN_COMPONENT(ColorGradientDemo)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoAntialiasableTriangleStage)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoAntialiasingPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoDOFCubeStage)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoDOFPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoMultiFramePipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoSSAOPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoSSAOPostprocessingStage)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoSSAORenderingStage)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoTextRenderingPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoTransparencyPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoTransparencyStage)
    CPPEXPOSE_PLUGIN_COMPONENT(GlyphSequenceDemoStage)
    CPPEXPOSE_PLUGIN_COMPONENT(LightTestPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(LightTestStage)
    CPPEXPOSE_PLUGIN_COMPONENT(ShaderDemoPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(ShapeDemo)

CPPEXPOSE_PLUGIN_LIBRARY_END
