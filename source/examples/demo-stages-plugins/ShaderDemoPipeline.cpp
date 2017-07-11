
#include "ShaderDemoPipeline.h"

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/BlitStage.h>
#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/ProgramStage.h>
#include <gloperate/stages/base/ShaderStage.h>
#include <gloperate/stages/base/ClearStage.h>
#include <gloperate/stages/base/RasterizationStage.h>
#include <gloperate/stages/base/RenderPassStage.h>
#include <gloperate/rendering/Quad.h>

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
, m_renderPassStage(cppassist::make_unique<gloperate::RenderPassStage>(environment, "RenderPassStage"))
, m_rasterizationStage(cppassist::make_unique<gloperate::RasterizationStage>(environment, "RasterizationStage"))
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

    // Render pass stage with fixed camera
    addStage(m_renderPassStage.get());
    //m_renderPassStage->drawable = m_quad.get(); // Will be set in onContextInit
    m_renderPassStage->program << m_programStage->program;
    m_renderPassStage->depthTest = false;

    auto textureInput = m_renderPassStage->createInput<globjects::Texture *>("texColor");
    *textureInput << m_textureLoadStage->texture;

    // Rasterization stage
    addStage(m_rasterizationStage.get());
    m_rasterizationStage->renderInterface.viewport << canvasInterface.viewport;
    m_rasterizationStage->drawable << m_renderPassStage->renderPass;
    m_rasterizationStage->createInput("Color") << *createInput<gloperate::ColorRenderTarget *>("Color");
    auto rasterizedColorTarget = m_rasterizationStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");

    // Outputs
    *createOutput<gloperate::ColorRenderTarget *>("ColorOut") << *rasterizedColorTarget;
}

ShaderDemoPipeline::~ShaderDemoPipeline()
{
}

void ShaderDemoPipeline::onContextInit(gloperate::AbstractGLContext * context)
{
    m_quad = cppassist::make_unique<gloperate::Quad>();

    m_renderPassStage->drawable = m_quad.get();

    Pipeline::onContextInit(context);
}

void ShaderDemoPipeline::onContextDeinit(gloperate::AbstractGLContext *context)
{
    Pipeline::onContextDeinit(context);

    m_quad.reset();
}
