
#include <gloperate/stages/demos/DemoPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/MixerStage.h>
#include <gloperate/stages/demos/TimerStage.h>
#include <gloperate/stages/demos/SpinningRectStage.h>
#include <gloperate/stages/demos/ColorizeStage.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoPipeline, gloperate::Stage)


DemoPipeline::DemoPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, texture("texture", this)
, angle("angle", this)
, rotate("rotate", this)
, color("color", this, glm::vec4(1.0, 0.0, 0.0, 1.0))
, m_textureLoadStage(new TextureLoadStage(environment, "TextureLoadStage"))
, m_timerStage(new TimerStage(environment, "TimerStage"))
, m_framebufferStage1(new BasicFramebufferStage(environment, "FramebufferStage1"))
, m_spinningRectStage(new SpinningRectStage(environment, "SpinningRectStage"))
, m_framebufferStage2(new BasicFramebufferStage(environment, "FramebufferStage2"))
, m_colorizeStage(new ColorizeStage(environment, "ColorizeStage"))
, m_mixerStage(new MixerStage(environment, "MixerStage"))
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

    // Timer stage
    addStage(m_timerStage);
    m_timerStage->timeDelta << renderInterface.timeDelta;

    // Framebuffer stage for spinning rect
    addStage(m_framebufferStage1);
    m_framebufferStage1->viewport << renderInterface.deviceViewport;

    // Spinning rectangle stage
    addStage(m_spinningRectStage);
    m_spinningRectStage->renderInterface.deviceViewport  << renderInterface.deviceViewport;
    m_spinningRectStage->renderInterface.targetFBO       << m_framebufferStage1->fbo;
    m_spinningRectStage->colorTexture                    << m_framebufferStage1->colorTexture;
    m_spinningRectStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_spinningRectStage->texture                         << m_textureLoadStage->texture;
    m_spinningRectStage->angle                           << m_timerStage->virtualTime;

    // Framebuffer stage for colorized output
    addStage(m_framebufferStage2);
    m_framebufferStage2->viewport << renderInterface.deviceViewport;

    // Colorize stage
    addStage(m_colorizeStage);
    m_colorizeStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_colorizeStage->renderInterface.targetFBO      << m_framebufferStage2->fbo;
    m_colorizeStage->colorTexture                   << m_framebufferStage2->colorTexture;
    m_colorizeStage->texture                        << m_spinningRectStage->colorTextureOut;
    m_colorizeStage->color                          << this->color;

    // Mixer stage
    addStage(m_mixerStage);
    m_mixerStage->viewport  << renderInterface.deviceViewport;
    m_mixerStage->targetFBO << renderInterface.targetFBO;
    m_mixerStage->texture   << m_colorizeStage->colorTextureOut;

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
