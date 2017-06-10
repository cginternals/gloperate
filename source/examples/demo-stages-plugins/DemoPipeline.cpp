
#include "DemoPipeline.h"

#include <glbinding/gl/enum.h>

#include <gloperate/gloperate.h>

#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/FramebufferStage.h>
#include <gloperate/stages/base/TextureStage.h>
#include <gloperate/stages/base/ProgramStage.h>
#include <gloperate/stages/base/RenderPassStage.h>
#include <gloperate/stages/base/RasterizationStage.h>
#include <gloperate/stages/base/ClearStage.h>
#include <gloperate/stages/base/ShapeStage.h>
#include <gloperate/stages/navigation/TrackballStage.h>
#include <gloperate/rendering/Shape.h>
#include <gloperate/rendering/Quad.h>

#include "TimerStage.h"


CPPEXPOSE_COMPONENT(DemoPipeline, gloperate::Stage)


using namespace cppexpose;
using namespace gloperate;


DemoPipeline::DemoPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, "DemoPipeline", name)
, renderInterface(this)
, shape("shape", this, ShapeType::Box)
, texture("texture", this)
, angle("angle", this)
, rotate("rotate", this)
, color("color", this, gloperate::Color(255, 255, 255, 255))
, m_timer(cppassist::make_unique<TimerStage>(environment, "Timer"))
, m_trackball(cppassist::make_unique<gloperate::TrackballStage>(environment, "Trackball"))
, m_shape(cppassist::make_unique<gloperate::ShapeStage>(environment, "Shape"))
, m_texture(cppassist::make_unique<gloperate::TextureLoadStage>(environment, "Texture"))
, m_framebuffer(cppassist::make_unique<gloperate::BasicFramebufferStage>(environment, "Framebuffer"))
, m_clear(cppassist::make_unique<gloperate::ClearStage>(environment, "Clear"))
, m_shapeProgram(cppassist::make_unique<gloperate::ProgramStage>(environment, "ShapeProgram"))
, m_shapeRenderPass(cppassist::make_unique<gloperate::RenderPassStage>(environment, "ShapeRenderPass"))
, m_shapeRasterization(cppassist::make_unique<gloperate::RasterizationStage>(environment, "ShapeRasterization"))
, m_colorizeProgram(cppassist::make_unique<gloperate::ProgramStage>(environment, "ColorizeProgram"))
, m_colorizeRenderPass(cppassist::make_unique<gloperate::RenderPassStage>(environment, "ColorizeRenderPass"))
, m_colorizeRasterization(cppassist::make_unique<gloperate::RasterizationStage>(environment, "ColorizeRasterization"))
{
    // Get data path
    std::string dataPath = gloperate::dataPath();

    // Setup parameters
    texture = dataPath + "/gloperate/textures/gloperate-logo.glraw";

    angle.setOption("minimumValue", 0.0f);
    angle.setOption("maximumValue", 6.3f);

    rotate = true;
    rotate.valueChanged.connect(this, &DemoPipeline::onRotateChanged);

    // Timer stage
    addStage(m_timer.get());
    m_timer->timeDelta << renderInterface.timeDelta;

    // Trackball stage
    addStage(m_trackball.get());
    m_trackball->viewport << renderInterface.deviceViewport;

    // Shape stage
    addStage(m_shape.get());
    m_shape->shapeType << this->shape;
    m_shape->width     = 1.0f;
    m_shape->height    = 1.0f;
    m_shape->depth     = 1.0f;
    m_shape->radius    = 1.0f;
    m_shape->texCoords = true;

    // Texture loader stage
    addStage(m_texture.get());
    m_texture->filename << texture;

    // Framebuffer stage
    addStage(m_framebuffer.get());
    m_framebuffer->viewport << renderInterface.deviceViewport;

    // Clear stage
    addStage(m_clear.get());
    m_clear->renderInterface.targetFBO << m_framebuffer->fbo;
    m_clear->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_clear->renderInterface.backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    m_clear->colorTexture << m_framebuffer->colorTexture;
    m_clear->createInput("renderPass") << m_shapeRenderPass->renderPass;

    // Program stage for shape
    addStage(m_shapeProgram.get());
    *m_shapeProgram->createInput<cppassist::FilePath>("vertexShader")   = dataPath + "/gloperate/shaders/geometry/geometry.vert";
    *m_shapeProgram->createInput<cppassist::FilePath>("fragmentShader") = dataPath + "/gloperate/shaders/geometry/geometry.frag";

    // Render pass stage for shape
    addStage(m_shapeRenderPass.get());
    m_shapeRenderPass->culling = false;
    m_shapeRenderPass->drawable << m_shape->drawable;
    m_shapeRenderPass->program << m_shapeProgram->program;
    m_shapeRenderPass->camera << m_trackball->camera;
    m_shapeRenderPass->createInput("color") << this->color;
    m_shapeRenderPass->createInput("tex0") << m_texture->texture;

    // Rasterization stage for shape
    addStage(m_shapeRasterization.get());
    m_shapeRasterization->renderInterface.targetFBO << m_clear->fboOut;
    m_shapeRasterization->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_shapeRasterization->drawable << m_shapeRenderPass->renderPass;
    m_shapeRasterization->colorTexture << m_clear->colorTextureOut;

    // Colorize program stage
    addStage(m_colorizeProgram.get());
    *m_colorizeProgram->createInput<cppassist::FilePath>("shader1") = dataPath + "/gloperate/shaders/geometry/screenaligned.vert";
    *m_colorizeProgram->createInput<cppassist::FilePath>("shader2") = dataPath + "/gloperate/shaders/demo/colorize.frag";

    // Colorize render pass stage
    addStage(m_colorizeRenderPass.get());
    // m_colorizeRenderPass->drawable is set in onContextInit()
    m_colorizeRenderPass->program << m_colorizeProgram->program;
    m_colorizeRenderPass->culling = false;
    m_colorizeRenderPass->createInput("color") << this->color;
    m_colorizeRenderPass->createInput("source") << m_shapeRasterization->colorTextureOut;

    // Colorize rasterization stage
    addStage(m_colorizeRasterization.get());
    m_colorizeRasterization->renderInterface.targetFBO << renderInterface.targetFBO;
    m_colorizeRasterization->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_colorizeRasterization->drawable << m_colorizeRenderPass->renderPass;

    // Outputs
    renderInterface.rendered << m_colorizeRasterization->renderInterface.rendered;
}

DemoPipeline::~DemoPipeline()
{
}

void DemoPipeline::onContextInit(gloperate::AbstractGLContext * context)
{
    Pipeline::onContextInit(context);

    m_quad = cppassist::make_unique<gloperate::Quad>(2.0f);

    m_colorizeRenderPass->drawable = m_quad.get();
}

void DemoPipeline::onContextDeinit(gloperate::AbstractGLContext * context)
{
    Pipeline::onContextDeinit(context);

    m_quad = nullptr;
}

void DemoPipeline::onRotateChanged(const bool & rotate)
{
    /*
    if (rotate) {
        m_spinningRectStage->angle << m_timer->virtualTime;
    } else {
        m_spinningRectStage->angle << angle;
    }
    */
}
