
#include "DemoPipeline.h"

#include <iostream>

#include <glbinding/gl/enum.h>

#include <gloperate/gloperate.h>

#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/FramebufferStage.h>
#include <gloperate/stages/base/MixerStage.h>
#include <gloperate/stages/base/TextureStage.h>
#include <gloperate/stages/base/ProgramStage.h>
#include <gloperate/stages/base/RenderPassStage.h>
#include <gloperate/stages/base/RasterizationStage.h>

#include <gloperate/rendering/Quad.h>

#include "TimerStage.h"
#include "SpinningRectStage.h"


CPPEXPOSE_COMPONENT(DemoPipeline, gloperate::Stage)


DemoPipeline::DemoPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, "DemoPipeline", name)
, renderInterface(this)
, texture("texture", this)
, angle("angle", this)
, rotate("rotate", this)
, color("color", this, gloperate::Color(255, 255, 255, 255))
, shader1("shader1", this)
, shader2("shader2", this)
, m_textureLoadStage(cppassist::make_unique<gloperate::TextureLoadStage>(environment, "TextureLoadStage"))
, m_timerStage(cppassist::make_unique<TimerStage>(environment, "TimerStage"))
, m_framebufferStage1(cppassist::make_unique<gloperate::BasicFramebufferStage>(environment, "FramebufferStage1"))
, m_spinningRectStage(cppassist::make_unique<SpinningRectStage>(environment, "SpinningRectStage"))
, m_framebufferStage2(cppassist::make_unique<gloperate::BasicFramebufferStage>(environment, "FramebufferStage2"))
, m_colorizeProgramStage(cppassist::make_unique<gloperate::ProgramStage>(environment, "ColorizeProgramStage"))
, m_colorizeRenderPassStage(cppassist::make_unique<gloperate::RenderPassStage>(environment, "ColorizeRenderPassStage"))
, m_colorizeRasterizationStage(cppassist::make_unique<gloperate::RasterizationStage>(environment, "ColorizeRasterizationStage"))
, m_mixerStage(cppassist::make_unique<gloperate::MixerStage>(environment, "MixerStage"))
{
    // Get data path
    std::string dataPath = gloperate::dataPath();

    // Setup parameters
    texture = dataPath + "/gloperate/textures/gloperate-logo.png";
    rotate  = true;
    rotate.valueChanged.connect(this, &DemoPipeline::onRotateChanged);

    // Texture loader stage
    addStage(m_textureLoadStage.get());
    m_textureLoadStage->filename << texture;

    // Timer stage
    addStage(m_timerStage.get());
    m_timerStage->timeDelta << renderInterface.timeDelta;

    // Framebuffer stage for spinning rect
    addStage(m_framebufferStage1.get());
    m_framebufferStage1->viewport << renderInterface.deviceViewport;

    // Spinning rectangle stage
    addStage(m_spinningRectStage.get());
    m_spinningRectStage->renderInterface.deviceViewport  << renderInterface.deviceViewport;
    m_spinningRectStage->renderInterface.targetFBO       << m_framebufferStage1->fbo;
    m_spinningRectStage->colorTexture                    << m_framebufferStage1->colorTexture;
    m_spinningRectStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_spinningRectStage->texture                         << m_textureLoadStage->texture;
    m_spinningRectStage->angle                           << m_timerStage->virtualTime;

    // Framebuffer stage for colorized output
    addStage(m_framebufferStage2.get());
    m_framebufferStage2->viewport << renderInterface.deviceViewport;

    shader1 = dataPath + "/gloperate/shaders/screenaligned/default.vert";
    shader2 = dataPath + "/gloperate/shaders/Demo/Colorize.frag";

    // Colorize program stage
    addStage(m_colorizeProgramStage.get());
    m_colorizeProgramStage->createInput("shader1") << shader1;
    m_colorizeProgramStage->createInput("shader2") << shader2;

    // Colorize render pass stage
    addStage(m_colorizeRenderPassStage.get());
    // m_colorizeRenderPassStage->drawable is set in onContextInit()
    m_colorizeRenderPassStage->program << m_colorizeProgramStage->program;
    m_colorizeRenderPassStage->createInput("color") << this->color;
    m_colorizeRenderPassStage->createInput("source") << m_spinningRectStage->colorTextureOut;

    // Colorize rasterization stage
    addStage(m_colorizeRasterizationStage.get());
    m_colorizeRasterizationStage->renderInterface.targetFBO << m_framebufferStage2->fbo;
    m_colorizeRasterizationStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_colorizeRasterizationStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_colorizeRasterizationStage->renderPass << m_colorizeRenderPassStage->renderPass;
    m_colorizeRasterizationStage->colorTexture << m_framebufferStage2->colorTexture;

    // Mixer stage
    addStage(m_mixerStage.get());
    m_mixerStage->viewport  << renderInterface.deviceViewport;
    m_mixerStage->targetFBO << renderInterface.targetFBO;
    m_mixerStage->texture   << m_colorizeRasterizationStage->colorTextureOut;

    // Outputs
    renderInterface.rendered << m_mixerStage->rendered;
}

DemoPipeline::~DemoPipeline()
{
}

void DemoPipeline::onContextInit(gloperate::AbstractGLContext * context)
{
    Pipeline::onContextInit(context);

    m_quad = cppassist::make_unique<gloperate::Quad>(2.0f);

std::cout << "DemoPipeline::onContextInit()" << std::endl;

    m_colorizeRenderPassStage->drawable = m_quad.get();
}

void DemoPipeline::onContextDeinit(gloperate::AbstractGLContext * context)
{
    Pipeline::onContextDeinit(context);

    m_quad = nullptr;
}

void DemoPipeline::onRotateChanged(const bool & rotate)
{
    if (rotate) {
        m_spinningRectStage->angle << m_timerStage->virtualTime;
    } else {
        m_spinningRectStage->angle << angle;
    }
}
