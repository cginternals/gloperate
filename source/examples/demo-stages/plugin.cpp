
#include <cppexpose/plugin/plugin_api.h>

#include "ColorizeStage.h"
#include "DemoAntialiasableTriangleStage.h"
#include "DemoAntialiasingPipeline.h"
#include "DemoDOFCubeStage.h"
#include "DemoDOFPipeline.h"
#include "DemoMultiFramePipeline.h"
#include "DemoPipeline.h"
#include "DemoRenderStage.h"
#include "DemoSSAOPipeline.h"
#include "DemoSSAOPostprocessingStage.h"
#include "DemoSSAORenderingStage.h"
#include "DemoStage.h"
#include "DemoTransparencyPipeline.h"
#include "DemoTransparencyStage.h"
#include "LightTestPipeline.h"
#include "LightTestStage.h"
#include "ShaderDemoPipeline.h"
#include "SpinningRectStage.h"
#include "TimerStage.h"


CPPEXPOSE_PLUGIN_LIBRARY

    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::ColorizeStage)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::DemoAntialiasableTriangleStage)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::DemoAntialiasingPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::DemoDOFCubeStage)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::DemoDOFPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::DemoMultiFramePipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::DemoPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::DemoRenderStage)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::DemoSSAOPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::DemoSSAOPostprocessingStage)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::DemoSSAORenderingStage)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::DemoStage)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::DemoTransparencyPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::DemoTransparencyStage)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::LightTestPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::LightTestStage)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::ShaderDemoPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::SpinningRectStage)
    CPPEXPOSE_PLUGIN_COMPONENT(gloperate::TimerStage)

CPPEXPOSE_PLUGIN_LIBRARY_END
