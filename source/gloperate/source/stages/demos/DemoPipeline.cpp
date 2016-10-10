
#include <gloperate/stages/demos/DemoPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/ProceduralTextureStage.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/MixerStage.h>
#include <gloperate/stages/base/SplitStage.h>
#include <gloperate/stages/demos/TimerStage.h>
#include <gloperate/stages/demos/SpinningRectStage.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoPipeline, gloperate::Stage)


DemoPipeline::DemoPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, texture("texture", this)
, angle("angle", this)
, rotate("rotate", this)
, m_mixerStage(new MixerStage(environment, "MixerStage"))
, m_splitStage(new SplitStage(environment, "SplitStage"))
, m_timerStage(new TimerStage(environment, "TimerStage"))
, m_spinningRectStage(new SpinningRectStage(environment, "SpinningRectStage"))
, m_framebufferStage(new BasicFramebufferStage(environment, "FramebufferStage"))
, m_textureLoadStage(new TextureLoadStage(environment, "TextureLoadStage"))
, m_proceduralTextureStage(new ProceduralTextureStage(environment, "ProceduralTextureStage"))
{
    // Get data path
    std::string dataPath = gloperate::dataPath();

    // Setup parameters
    *texture = dataPath + "/gloperate/textures/gloperate-logo.png";
    *rotate  = true;
    rotate.valueChanged.connect(this, &DemoPipeline::onRotateChanged);

    // Texture loader stage
    addStage(m_textureLoadStage);
    m_textureLoadStage->filename << texture;

    // Procedural texture stage
    addStage(m_proceduralTextureStage);

    // Framebuffer stage
    addStage(m_framebufferStage);
    m_framebufferStage->viewport << renderInterface.deviceViewport;

    // Timer stage
    addStage(m_timerStage);
    m_timerStage->timeDelta << renderInterface.timeDelta;

    // Triangle stage
    addStage(m_spinningRectStage);
    m_spinningRectStage->renderInterface.deviceViewport  << renderInterface.deviceViewport;
    m_spinningRectStage->renderInterface.targetFBO       << m_framebufferStage->fbo;
    m_spinningRectStage->colorTexture                    << m_framebufferStage->colorTexture;
    m_spinningRectStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_spinningRectStage->texture                         << m_textureLoadStage->texture;
    m_spinningRectStage->angle                           << m_timerStage->virtualTime;

    // Mixer stage
    addStage(m_mixerStage);
    m_mixerStage->viewport  << renderInterface.deviceViewport;
    m_mixerStage->targetFBO << renderInterface.targetFBO;
    m_mixerStage->texture   << m_spinningRectStage->colorTextureOut;

    // Split stage
    /*
    addStage(m_splitStage);
    m_splitStage->viewport  << renderInterface.deviceViewport;
    m_splitStage->targetFBO << renderInterface.targetFBO;
    m_splitStage->texture1  << m_spinningRectStage->colorTextureOut;
    m_splitStage->texture2  << m_proceduralTextureStage->texture;
    */

    // Outputs
    renderInterface.rendered << m_mixerStage->rendered;
}

DemoPipeline::~DemoPipeline()
{
}

void DemoPipeline::onRotateChanged(const bool & rotate)
{
    if (rotate) {
        m_spinningRectStage->angle << m_timerStage->virtualTime;
    } else {
        m_spinningRectStage->angle << angle;
    }
}


} // namespace gloperate
