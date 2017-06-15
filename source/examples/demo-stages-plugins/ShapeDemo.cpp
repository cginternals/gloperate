
#include "ShapeDemo.h"

#include <glbinding/gl/enum.h>

#include <gloperate/gloperate.h>

#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/TextureStage.h>
#include <gloperate/stages/base/ProgramStage.h>
#include <gloperate/stages/base/RenderPassStage.h>
#include <gloperate/stages/base/RasterizationStage.h>
#include <gloperate/stages/base/ClearStage.h>
#include <gloperate/stages/base/ShapeStage.h>
#include <gloperate/stages/base/TimerStage.h>
#include <gloperate/stages/base/TransformStage.h>
#include <gloperate/stages/navigation/TrackballStage.h>
#include <gloperate/rendering/Shape.h>
#include <gloperate/rendering/Quad.h>
#include <gloperate/rendering/RenderTarget.h>


CPPEXPOSE_COMPONENT(ShapeDemo, gloperate::Stage)


using namespace cppexpose;
using namespace gloperate;


ShapeDemo::ShapeDemo(Environment * environment, const std::string & name)
: Pipeline(environment, "ShapeDemo", name)
, canvasInterface(this)
, shape("shape", this, ShapeType::Box)
, texture("texture", this)
, angle("angle", this, 0.0f)
, rotate("rotate", this, false)
, color("color", this, Color(255, 255, 255, 255))
, m_colorTarget(cppassist::make_unique<gloperate::RenderTarget>())
, m_timer(cppassist::make_unique<TimerStage>(environment, "Timer"))
, m_trackball(cppassist::make_unique<TrackballStage>(environment, "Trackball"))
, m_shape(cppassist::make_unique<ShapeStage>(environment, "Shape"))
, m_texture(cppassist::make_unique<TextureLoadStage>(environment, "Texture"))
, m_framebuffer(cppassist::make_unique<BasicFramebufferStage>(environment, "Framebuffer"))
, m_clear(cppassist::make_unique<ClearStage>(environment, "Clear"))
, m_shapeTransform(cppassist::make_unique<TransformStage>(environment, "ShapeTransform"))
, m_shapeProgram(cppassist::make_unique<ProgramStage>(environment, "ShapeProgram"))
, m_shapeRenderPass(cppassist::make_unique<RenderPassStage>(environment, "ShapeRenderPass"))
, m_shapeRasterization(cppassist::make_unique<RasterizationStage>(environment, "ShapeRasterization"))
, m_colorizeProgram(cppassist::make_unique<ProgramStage>(environment, "ColorizeProgram"))
, m_colorizeRenderPass(cppassist::make_unique<RenderPassStage>(environment, "ColorizeRenderPass"))
, m_colorizeRasterization(cppassist::make_unique<RasterizationStage>(environment, "ColorizeRasterization"))
{
    m_colorTarget->setAttachmentType(gloperate::AttachmentType::Color);

    // Get data path
    std::string dataPath = gloperate::dataPath();

    // Setup parameters
    texture = dataPath + "/gloperate/textures/gloperate-logo.glraw";

    angle.setOption("minimumValue", 0.0f);
    angle.setOption("maximumValue", 2.0f * glm::pi<float>());
    angle.setOption("updateOnDrag", true);

    rotate.valueChanged.connect(this, &ShapeDemo::onRotateChanged);

    // Timer stage
    addStage(m_timer.get());
    m_timer->interval = 2.0f * glm::pi<float>();

    // Trackball stage
    addStage(m_trackball.get());
    m_trackball->viewport << canvasInterface.viewport;

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
    m_texture->filename << this->texture;

    // Framebuffer stage
    addStage(m_framebuffer.get());
    m_framebuffer->viewport << canvasInterface.viewport;

    // Clear stage
    addStage(m_clear.get());
    m_clear->createInput("ColorAttachment") << m_framebuffer->colorBuffer;
    m_clear->createInput("DepthAttachment") << m_framebuffer->depthBuffer;
    m_clear->renderInterface.viewport << canvasInterface.viewport;

    // Invalidation of Clear Stage if new rendering should take place
    m_clear->createInput("renderPass") << m_shapeRenderPass->renderPass;

    // Transform stage for shape
    addStage(m_shapeTransform.get());
    m_shapeTransform->rotationAngle << angle;

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
    m_shapeRenderPass->modelMatrix << m_shapeTransform->modelMatrix;
    m_shapeRenderPass->createInput("color") << this->color;
    m_shapeRenderPass->createInput("tex0") << m_texture->texture;

    // Rasterization stage for shape
    addStage(m_shapeRasterization.get());
    m_shapeRasterization->createInput("ColorAttachment") << *m_clear->createOutput<gloperate::RenderTarget *>("ColorAttachmentOut");
    m_shapeRasterization->createInput("DepthAttachment") << *m_clear->createOutput<gloperate::RenderTarget *>("DepthAttachmentOut");
    m_shapeRasterization->renderInterface.viewport << canvasInterface.viewport;
    m_shapeRasterization->drawable << m_shapeRenderPass->renderPass;

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
    m_colorizeRenderPass->createInput("source") << m_framebuffer->colorTexture;

    // Colorize rasterization stage
    addStage(m_colorizeRasterization.get());
    m_colorizeRasterization->createInput("ColorAttachment") << *createInput<gloperate::RenderTarget *>("Color");
    m_colorizeRasterization->renderInterface.viewport << canvasInterface.viewport;
    m_colorizeRasterization->drawable << m_colorizeRenderPass->renderPass;

    // Outputs
    *createOutput<gloperate::RenderTarget *>("ColorOut") << *m_colorizeRasterization->createOutput<gloperate::RenderTarget *>("ColorOut");

    // Start rotation
    rotate = true;
}

ShapeDemo::~ShapeDemo()
{
}

void ShapeDemo::onContextInit(AbstractGLContext * context)
{
    Pipeline::onContextInit(context);

    m_quad = cppassist::make_unique<Quad>(2.0f);

    m_colorizeRenderPass->drawable = m_quad.get();
}

void ShapeDemo::onContextDeinit(AbstractGLContext * context)
{
    Pipeline::onContextDeinit(context);

    m_quad = nullptr;
}

void ShapeDemo::onRotateChanged(const bool & rotate)
{
    // Switch rotation on
    if (rotate)
    {
        // Set timer to current rotation value
        m_timer->virtualTime = *angle;

        // Connect angle to timer and resume timer
        angle << m_timer->virtualTime;
        m_timer->timeDelta << canvasInterface.timeDelta;
    }

    // Switch rotation off
    else
    {
        // Set angle to current timer value
        angle.disconnect();
        angle = *m_timer->virtualTime;

        // Stop time
        m_timer->timeDelta.disconnect();
    }
}
