
#include <gloperate/stages/demos/DemoPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/MixerStage.h>
#include <gloperate/stages/demos/TimerStage.h>
#include <gloperate/stages/demos/SpinningRectStage.h>
#include <gloperate/stages/demos/ColorizeStage.h>

#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/ShaderLoaderStage.h>
#include <gloperate/stages/base/ShaderStage.h>
#include <gloperate/stages/base/ProgramStage.h>
#include <gloperate/stages/demos/DemoRenderStage.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoPipeline, gloperate::Stage)


DemoPipeline::DemoPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, shader1("shader1", this)
, shader2("shader2", this)
, texture("texture", this)
, m_textureLoadStage(new TextureLoadStage(environment, "TextureLoadStage"))
, m_shaderLoadStage1(new ShaderLoaderStage(environment, "ShaderLoaderStage1"))
, m_shaderStage1(new ShaderStage(environment, "ShaderStage1"))
, m_shaderLoadStage2(new ShaderLoaderStage(environment, "ShaderLoaderStage2"))
, m_shaderStage2(new ShaderStage(environment, "ShaderStage2"))
, m_programStage(new ProgramStage(environment, "ProgramStage"))
, m_framebufferStage(new BasicFramebufferStage(environment, "BasicFramebufferStage"))
, m_renderStage(new DemoRenderStage(environment, "RenderStage"))
, m_mixerStage(new MixerStage(environment, "MixerStage"))
{
    // Get data path
    std::string dataPath = gloperate::dataPath();

    // Setup parameters
    *shader1 = dataPath + "/gloperate/shaders/Demo/Demo.frag";
    *shader2 = dataPath + "/gloperate/shaders/Demo/Demo.vert";

    *texture = dataPath + "/gloperate/textures/gloperate-logo.png";

    // Texture loader stage
    addStage(m_textureLoadStage);
    m_textureLoadStage->filename << texture;

    // Shader loading
    addStage(m_shaderLoadStage1);
    addStage(m_shaderStage1);
    m_shaderLoadStage1->filePath << shader1;
    m_shaderStage1->type << m_shaderLoadStage1->type;
    m_shaderStage1->source << m_shaderLoadStage1->source;

    // Second shader
    addStage(m_shaderLoadStage2);
    addStage(m_shaderStage2);
    m_shaderLoadStage2->filePath << shader2;
    m_shaderStage2->type << m_shaderLoadStage2->type;
    m_shaderStage2->source << m_shaderLoadStage2->source;

    // Timer stage
    addStage(m_programStage);
    *(m_programStage->createInput<globjects::Shader *>("shader1")) << m_shaderStage1->shader;
    *(m_programStage->createInput<globjects::Shader *>("shader2")) << m_shaderStage2->shader;

    // Framebuffer stage for spinning rect
    addStage(m_framebufferStage);
    m_framebufferStage->viewport << renderInterface.deviceViewport;

    // Spinning rectangle stage
    addStage(m_renderStage);
    m_renderStage->renderInterface.deviceViewport  << renderInterface.deviceViewport;
    m_renderStage->renderInterface.targetFBO       << m_framebufferStage->fbo;
    m_renderStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_renderStage->colorTexture                    << m_framebufferStage->colorTexture;
    m_renderStage->texture                         << m_textureLoadStage->texture;
    m_renderStage->program                         << m_programStage->program;

    // Mixer stage
    addStage(m_mixerStage);
    m_mixerStage->viewport  << renderInterface.deviceViewport;
    m_mixerStage->targetFBO << renderInterface.targetFBO;
    m_mixerStage->texture   << m_renderStage->colorTextureOut;

    // Outputs
    renderInterface.rendered << m_mixerStage->rendered;
}

DemoPipeline::~DemoPipeline()
{
}

} // namespace gloperate
