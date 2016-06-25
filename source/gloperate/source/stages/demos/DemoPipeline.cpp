
#include <gloperate/stages/demos/DemoPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/ProceduralTextureStage.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/MixerStage.h>
#include <gloperate/stages/base/SplitStage.h>
#include <gloperate/stages/demos/DemoTimerStage.h>
#include <gloperate/stages/demos/DemoTriangleStage.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoPipeline, gloperate::Stage)


DemoPipeline::DemoPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, m_mixerStage(new MixerStage(environment, "MixerStage"))
, m_splitStage(new SplitStage(environment, "SplitStage"))
, m_timerStage(new DemoTimerStage(environment, "TimerStage"))
, m_triangleStage(new DemoTriangleStage(environment, "TriangleStage"))
, m_framebufferStage(new BasicFramebufferStage(environment, "FramebufferStage"))
, m_textureLoadStage(new TextureLoadStage(environment, "TextureLoadStage"))
, m_proceduralTextureStage(new ProceduralTextureStage(environment, "ProceduralTextureStage"))
{
    // Get data path
    std::string dataPath = gloperate::dataPath();

    // Texture loader stage
    addStage(m_textureLoadStage);
    *m_textureLoadStage->filename = dataPath + "/gloperate/textures/gloperate-logo.png";

    // Procedural texture stage
    addStage(m_proceduralTextureStage);

    // Framebuffer stage
    addStage(m_framebufferStage);
    m_framebufferStage->viewport << renderInterface.deviceViewport;

    // Timer stage
    addStage(m_timerStage);
    m_timerStage->timeDelta << renderInterface.timeDelta;

    // Triangle stage
    addStage(m_triangleStage);
    m_triangleStage->renderInterface.deviceViewport  << renderInterface.deviceViewport;
    m_triangleStage->renderInterface.targetFBO       << m_framebufferStage->fbo;
    m_triangleStage->colorTexture                    << m_framebufferStage->colorTexture;
    m_triangleStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_triangleStage->texture                         << m_textureLoadStage->texture;
    m_triangleStage->angle                           << m_timerStage->virtualTime;

    // Mixer stage
    addStage(m_mixerStage);
//  m_mixerStage->viewport  << this->deviceViewport;
//  m_mixerStage->targetFBO << this->targetFBO;
//  m_mixerStage->texture   << m_framebufferStage->colorTexture;

    // Split stage
    addStage(m_splitStage);
    m_splitStage->viewport  << renderInterface.deviceViewport;
    m_splitStage->targetFBO << renderInterface.targetFBO;
    m_splitStage->texture1  << m_triangleStage->colorTextureOut;
    m_splitStage->texture2  << m_proceduralTextureStage->texture;

    // Outputs
    renderInterface.rendered << m_splitStage->rendered;
}

DemoPipeline::~DemoPipeline()
{
}


} // namespace gloperate
