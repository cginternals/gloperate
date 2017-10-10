
#include "TransparencyRenderingPipeline.h"

#include <glbinding/gl/enum.h>

#include <globjects/base/File.h>

#include <gloperate/gloperate.h>
#include <gloperate/rendering/Drawable.h>
#include <gloperate/rendering/ColorRenderTarget.h>
#include <gloperate/stages/base/ProgramStage.h>
#include <gloperate/stages/base/ClearStage.h>
#include <gloperate/stages/base/RenderPassStage.h>
#include <gloperate/stages/base/RasterizationStage.h>

#include <gloperate-glkernel/stages/TransparencyKernelStage.h>


CPPEXPOSE_COMPONENT(TransparencyRenderingPipeline, gloperate::Stage)


TransparencyRenderingPipeline::TransparencyRenderingPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, canvasInterface(this)
, m_transparencyKernelStage(cppassist::make_unique<gloperate_glkernel::TransparencyKernelStage>(environment))
, m_programStage(cppassist::make_unique<gloperate::ProgramStage>(environment))
, m_clearStage(cppassist::make_unique<gloperate::ClearStage>(environment))
, m_renderPassStage(cppassist::make_unique<gloperate::RenderPassStage>(environment))
, m_rasterizationStage(cppassist::make_unique<gloperate::RasterizationStage>(environment))
{
    auto dataPath = gloperate::dataPath();

    addStage(m_transparencyKernelStage.get());
    m_transparencyKernelStage->kernelSize = glm::ivec2(256, 256);

    addStage(m_programStage.get());
    *m_programStage->createInput<cppassist::FilePath>("vertexShader")   = dataPath + "/gloperate/shaders/demos/transparency.vert";
    *m_programStage->createInput<cppassist::FilePath>("fragmentShader") = dataPath + "/gloperate/shaders/demos/transparency.frag";

    addStage(m_renderPassStage.get());
    //m_renderPassStage->drawable will be set in onContextInit
    m_renderPassStage->program << m_programStage->program;
    m_renderPassStage->depthTest = false;
    m_renderPassStage->createInput("currentFrame") << canvasInterface.frameCounter;
    m_renderPassStage->createInput("timeDelta") << canvasInterface.timeDelta;
    m_renderPassStage->createInput("transparencyKernel") << m_transparencyKernelStage->texture;

    addStage(m_clearStage.get());
    m_clearStage->createInput("Color") << *createInput<gloperate::ColorRenderTarget *>("Color");
    m_clearStage->createInput("ColorValue") = gloperate::Color(0.0f, 0.0f, 0.0f);
    m_clearStage->renderInterface.viewport << canvasInterface.viewport;
    m_clearStage->createInput("ClearTrigger") << m_renderPassStage->renderPass;

    addStage(m_rasterizationStage.get());
    m_rasterizationStage->createInput("Color") << *m_clearStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");
    m_rasterizationStage->renderInterface.viewport << canvasInterface.viewport;
    m_rasterizationStage->drawable << m_renderPassStage->renderPass;

    *createOutput<gloperate::ColorRenderTarget *>("ColorOut") << *m_rasterizationStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");
}

TransparencyRenderingPipeline::~TransparencyRenderingPipeline()
{
}

void TransparencyRenderingPipeline::onContextInit(gloperate::AbstractGLContext * context)
{
    using vertexData = std::tuple<glm::vec2, glm::vec2, glm::vec4>;
    //                            position   uv         color

    m_drawable.reset(new gloperate::Drawable());

    // circles, consisting of two triangles each
    // sorted from back to front to render correctly without depth buffer
    static const std::vector<vertexData> vertices {
        // blue, center at (0.2598, -0.15)
        {{-0.2402f,  0.35f}, {-1.0f,  1.0f}, {0.0f, 0.0f, 1.0f, 0.8f}},
        {{-0.2402f, -0.65f}, {-1.0f, -1.0f}, {0.0f, 0.0f, 1.0f, 0.8f}},
        {{ 0.7598f, -0.65f}, { 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f, 0.8f}},
        {{ 0.7598f, -0.65f}, { 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f, 0.8f}},
        {{ 0.7598f,  0.35f}, { 1.0f,  1.0f}, {0.0f, 0.0f, 1.0f, 0.8f}},
        {{-0.2402f,  0.35f}, {-1.0f,  1.0f}, {0.0f, 0.0f, 1.0f, 0.8f}},
        // green, center at (-0.2598, -0.15)
        {{-0.7598f,  0.35f}, {-1.0f,  1.0f}, {0.0f, 1.0f, 0.0f, 0.5f}},
        {{-0.7598f, -0.65f}, {-1.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 0.5f}},
        {{ 0.2402f, -0.65f}, { 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 0.5f}},
        {{ 0.2402f, -0.65f}, { 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 0.5f}},
        {{ 0.2402f,  0.35f}, { 1.0f,  1.0f}, {0.0f, 1.0f, 0.0f, 0.5f}},
        {{-0.7598f,  0.35f}, {-1.0f,  1.0f}, {0.0f, 1.0f, 0.0f, 0.5f}},
        // red, center at (0.0, 0.3)
        {{-0.5f,  0.8f}, {-1.0f,  1.0f}, {1.0f, 0.0f, 0.0f, 0.3f}},
        {{-0.5f, -0.2f}, {-1.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 0.3f}},
        {{ 0.5f, -0.2f}, { 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 0.3f}},
        {{ 0.5f, -0.2f}, { 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 0.3f}},
        {{ 0.5f,  0.8f}, { 1.0f,  1.0f}, {1.0f, 0.0f, 0.0f, 0.3f}},
        {{-0.5f,  0.8f}, {-1.0f,  1.0f}, {1.0f, 0.0f, 0.0f, 0.3f}},
    };

    globjects::Buffer * vertexBuffer = new globjects::Buffer;
    m_drawable->setBuffer(0, vertexBuffer);
    m_drawable->setData(0, vertices, gl::GL_STATIC_DRAW);

    m_drawable->bindAttribute(0, 0);
    m_drawable->bindAttribute(1, 1);
    m_drawable->bindAttribute(2, 2);
    m_drawable->setAttributeBindingBuffer(0, 0, 0, sizeof(vertexData));
    m_drawable->setAttributeBindingBuffer(1, 0, 0, sizeof(vertexData));
    m_drawable->setAttributeBindingBuffer(2, 0, 0, sizeof(vertexData));
    // std::tuples contain their data in reverse order!
    m_drawable->setAttributeBindingFormat(0, 2, gl::GL_FLOAT, gl::GL_FALSE, sizeof(glm::vec4) + sizeof(glm::vec2));
    m_drawable->setAttributeBindingFormat(1, 2, gl::GL_FLOAT, gl::GL_FALSE, sizeof(glm::vec4));
    m_drawable->setAttributeBindingFormat(2, 4, gl::GL_FLOAT, gl::GL_FALSE, 0);

    m_drawable->enableAllAttributeBindings();
    m_drawable->setSize(vertices.size());

    m_renderPassStage->drawable = m_drawable.get();

    m_randomIncludeNamedString = globjects::NamedString::create("/gloperate/shaders/util/random.glsl", new globjects::File(gloperate::dataPath() + "/gloperate/shaders/util/random.glsl"));

    Pipeline::onContextInit(context);
}

void TransparencyRenderingPipeline::onContextDeinit(gloperate::AbstractGLContext * context)
{
    Pipeline::onContextDeinit(context);

    m_randomIncludeNamedString.reset();
    m_drawable.reset();
}
