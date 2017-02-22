
#include <gloperate/stages/demos/DemoPipeline.h>

#include <glbinding/gl/enum.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/FramebufferStage.h>
#include <gloperate/stages/base/MixerStage.h>
#include <gloperate/stages/base/TextureStage.h>
#include <gloperate/stages/demos/TimerStage.h>
#include <gloperate/stages/demos/SpinningRectStage.h>
#include <gloperate/stages/demos/ColorizeStage.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoPipeline, gloperate::Stage)


DemoPipeline::DemoPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, "DemoPipeline", name)
, renderInterface(this)
, texture("texture", this)
, angle("angle", this)
, rotate("rotate", this)
, color("color", this, Color(255, 255, 255, 255))
, m_textureLoadStage(createStage<TextureLoadStage>(environment, "TextureLoadStage"))
, m_timerStage(createStage<TimerStage>(environment, "TimerStage"))
, m_framebufferStage1(createStage<BasicFramebufferStage>(environment, "FramebufferStage1"))
, m_spinningRectStage(createStage<SpinningRectStage>(environment, "SpinningRectStage"))
, m_textureStage1(createStage<TextureStage>(environment, "TextureStage1"))
, m_textureStage2(createStage<TextureStage>(environment, "TextureStage2"))
, m_framebufferStage2(createStage<FramebufferStage>(environment, "FramebufferStage2"))
, m_colorizeStage(createStage<ColorizeStage>(environment, "ColorizeStage"))
, m_mixerStage(createStage<MixerStage>(environment, "MixerStage"))
{
    // Get data path
    std::string dataPath = gloperate::dataPath();

    // Setup parameters
    texture = dataPath + "/gloperate/textures/gloperate-logo.png";
    rotate  = true;
    rotate.valueChanged.connect(this, &DemoPipeline::onRotateChanged);

    // Texture loader stage
    m_textureLoadStage->filename << texture;

    // Timer stage
    m_timerStage->timeDelta << renderInterface.timeDelta;

    // Framebuffer stage for spinning rect
    m_framebufferStage1->viewport << renderInterface.deviceViewport;

    // Spinning rectangle stage
    m_spinningRectStage->renderInterface.deviceViewport  << renderInterface.deviceViewport;
    m_spinningRectStage->renderInterface.targetFBO       << m_framebufferStage1->fbo;
    m_spinningRectStage->colorTexture                    << m_framebufferStage1->colorTexture;
    m_spinningRectStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_spinningRectStage->texture                         << m_textureLoadStage->texture;
    m_spinningRectStage->angle                           << m_timerStage->virtualTime;

    // textures 1 for 2nd frame buffer
    m_textureStage1->internalFormat.setValue(gl::GL_RGBA);
    m_textureStage1->format.setValue(gl::GL_RGBA);
    m_textureStage1->type.setValue(gl::GL_UNSIGNED_BYTE);
    m_textureStage1->size << renderInterface.deviceViewport;

    // textures 2 for 2nd frame buffer
    m_textureStage2->internalFormat.setValue(gl::GL_DEPTH_COMPONENT);
    m_textureStage2->format.setValue(gl::GL_DEPTH_COMPONENT);
    m_textureStage2->type.setValue(gl::GL_UNSIGNED_BYTE);
    m_textureStage2->size << renderInterface.deviceViewport;

    // Framebuffer stage for colorized output
    m_framebufferStage2->colorTexture << m_textureStage1->renderTarget;
    m_framebufferStage2->depthTexture << m_textureStage2->renderTarget;

    // Colorize stage
    m_colorizeStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_colorizeStage->renderInterface.targetFBO      << m_framebufferStage2->fbo;
    m_colorizeStage->colorTexture                   << m_textureStage1->texture;
    m_colorizeStage->texture                        << m_spinningRectStage->colorTextureOut;
    m_colorizeStage->color                          << this->color;

    // Mixer stage
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
