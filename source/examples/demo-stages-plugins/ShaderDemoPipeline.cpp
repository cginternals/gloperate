
#include "ShaderDemoPipeline.h"

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/BlitStage.h>
#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/ProgramStage.h>
#include <gloperate/stages/base/ShaderStage.h>
#include <gloperate/stages/base/RasterizationStage.h>
#include <gloperate/stages/base/RenderPassStage.h>

#include "DemoDrawableStage.h"

#include <cppassist/logging/logging.h>


CPPEXPOSE_COMPONENT(ShaderDemoPipeline, gloperate::Stage)


ShaderDemoPipeline::ShaderDemoPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, canvasInterface(this)
, shader1("shader1", this)
, shader2("shader2", this)
, texture("texture", this)
, m_textureLoadStage(cppassist::make_unique<gloperate::TextureLoadStage>(environment, "TextureLoadStage"))
, m_shaderStage(cppassist::make_unique<gloperate::ShaderStage>(environment, "ShaderStage"))
, m_programStage(cppassist::make_unique<gloperate::ProgramStage>(environment, "ProgramStage"))
, m_framebufferStage(cppassist::make_unique<gloperate::BasicFramebufferStage>(environment, "BasicFramebufferStage"))
, m_demoDrawableStage(cppassist::make_unique<DemoDrawableStage>(environment, "DemoDrawableStage"))
, m_renderPassStage(cppassist::make_unique<gloperate::RenderPassStage>(environment, "RenderPassStage"))
, m_rasterizationStage(cppassist::make_unique<gloperate::RasterizationStage>(environment, "RasterizationStage"))
, m_blitStage(cppassist::make_unique<gloperate::BlitStage>(environment, "BlitStage"))
{
    // Get data path
    std::string dataPath = gloperate::dataPath();

    // Setup parameters
    shader1 = dataPath + "/gloperate/shaders/demos/demo.frag";
    shader2 = dataPath + "/gloperate/shaders/demos/demo.vert";

    texture = dataPath + "/gloperate/textures/gloperate-logo.png";

    // Texture loader stage
    addStage(m_textureLoadStage.get());
    m_textureLoadStage->filename << texture;

    // Shader stage
    addStage(m_shaderStage.get());
    m_shaderStage->filePath << shader2;

    // Basic program stage
    addStage(m_programStage.get());
    m_programStage->createInput("shader1") << shader1;
    m_programStage->createInput("shader2") << m_shaderStage->shader;

    // Framebuffer stage
    addStage(m_framebufferStage.get());
    m_framebufferStage->viewport << canvasInterface.viewport;

    // Demo drawable stage (supplies demo drawable)
    addStage(m_demoDrawableStage.get());

    // Render pass stage with fixed camera
    addStage(m_renderPassStage.get());
    // m_renderPassStage->camera.setValue(new Camera(glm::vec3(0.0, 0.0, 12.0)));
    m_renderPassStage->drawable << m_demoDrawableStage->drawable;
    m_renderPassStage->program << m_programStage->program;

    auto textureInput = m_renderPassStage->createInput<globjects::Texture *>("texColor");
    *textureInput << m_textureLoadStage->texture;

    // Rasterization stage
    addStage(m_rasterizationStage.get());
    m_rasterizationStage->renderInterface.viewport << canvasInterface.viewport;
    m_rasterizationStage->drawable << m_renderPassStage->renderPass;
    m_rasterizationStage->createInput("Color") << m_framebufferStage->colorBuffer;
    m_rasterizationStage->createInput("Depth") << m_framebufferStage->depthBuffer;

    // Blit stage
    addStage(m_blitStage.get());
    m_blitStage->source << *m_rasterizationStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");
    m_blitStage->sourceViewport << canvasInterface.viewport;
    m_blitStage->target << *createInput<gloperate::ColorRenderTarget *>("Color");
    m_blitStage->targetViewport << canvasInterface.viewport;

    // Outputs
    *createOutput<gloperate::ColorRenderTarget *>("ColorOut") << *m_blitStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");
}

ShaderDemoPipeline::~ShaderDemoPipeline()
{
}
