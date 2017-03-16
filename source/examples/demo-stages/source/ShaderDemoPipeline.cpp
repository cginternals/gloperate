
#include <demo-stages/ShaderDemoPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/MixerStage.h>
#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/ProgramStage.h>
#include <gloperate/stages/base/ShaderStage.h>
#include <gloperate/stages/base/RasterizationStage.h>
#include <gloperate/stages/base/RenderPassStage.h>

#include <demo-stages/DemoDrawableStage.h>

#include <cppassist/logging/logging.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ShaderDemoPipeline, gloperate::Stage)


ShaderDemoPipeline::ShaderDemoPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, shader1("shader1", this)
, shader2("shader2", this)
, texture("texture", this)
, m_textureLoadStage(cppassist::make_unique<TextureLoadStage>(environment, "TextureLoadStage"))
, m_shaderStage(cppassist::make_unique<ShaderStage>(environment, "ShaderStage"))
, m_programStage(cppassist::make_unique<ProgramStage>(environment, "ProgramStage"))
, m_framebufferStage(cppassist::make_unique<BasicFramebufferStage>(environment, "BasicFramebufferStage"))
, m_demoDrawableStage(cppassist::make_unique<DemoDrawableStage>(environment, "DemoDrawableStage"))
, m_renderPassStage(cppassist::make_unique<RenderPassStage>(environment, "RenderPassStage"))
, m_rasterizationStage(cppassist::make_unique<RasterizationStage>(environment, "RasterizationStage"))
, m_mixerStage(cppassist::make_unique<MixerStage>(environment, "MixerStage"))
{
    setVerbosityLevel(cppassist::LogMessage::Debug);

    // Get data path
    std::string dataPath = gloperate::dataPath();

    // Setup parameters
    shader1 = dataPath + "/gloperate/shaders/Demo/Demo.frag";
    shader2 = dataPath + "/gloperate/shaders/Demo/Demo.vert";

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
    m_framebufferStage->viewport << renderInterface.deviceViewport;

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
    m_rasterizationStage->renderInterface.targetFBO << m_framebufferStage->fbo;
    m_rasterizationStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_rasterizationStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_rasterizationStage->renderPass << m_renderPassStage->renderPass;
    m_rasterizationStage->colorTexture << m_framebufferStage->colorTexture;

    // Mixer stage
    addStage(m_mixerStage.get());
    m_mixerStage->viewport  << renderInterface.deviceViewport;
    m_mixerStage->targetFBO << renderInterface.targetFBO;
    m_mixerStage->texture   << m_rasterizationStage->colorTextureOut;

    // Outputs
    renderInterface.rendered << m_mixerStage->rendered;
}

ShaderDemoPipeline::~ShaderDemoPipeline()
{
}

} // namespace gloperate
