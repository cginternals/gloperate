
#include <cppexpose/plugin/plugin_api.h>

#include <demo-stages/ColorizeStage.h>
#include <demo-stages/DemoAntialiasableTriangleStage.h>
#include <demo-stages/DemoAntialiasingPipeline.h>
#include <demo-stages/DemoDOFCubeStage.h>
#include <demo-stages/DemoDOFPipeline.h>
#include <demo-stages/DemoMultiFramePipeline.h>
#include <demo-stages/DemoPipeline.h>
#include <demo-stages/DemoRenderStage.h>
#include <demo-stages/DemoSSAOPipeline.h>
#include <demo-stages/DemoSSAOPostprocessingStage.h>
#include <demo-stages/DemoSSAORenderingStage.h>
#include <demo-stages/DemoStage.h>
#include <demo-stages/DemoTransparencyPipeline.h>
#include <demo-stages/DemoTransparencyStage.h>
#include <demo-stages/LightTestPipeline.h>
#include <demo-stages/LightTestStage.h>
#include <demo-stages/ShaderDemoPipeline.h>
#include <demo-stages/SpinningRectStage.h>
#include <demo-stages/TimerStage.h>


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
