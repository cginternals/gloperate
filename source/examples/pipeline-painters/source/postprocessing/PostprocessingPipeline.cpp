
#include <pipeline-painters/postprocessing/PostprocessingPipeline.h>

#include <glm/gtc/constants.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/base/File.h>
#include <globjects/logging.h>

#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>
#include <globjects/Renderbuffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>

#include <gloperate/Camera.h>

#include <gloperate/pipelines/AbstractStage.h>
#include <gloperate/pipelines/Data.h>
#include <gloperate/pipelines/InputSlot.h>

#include <gloperate/capabilities/AbstractViewportCapability.h>
#include <gloperate/capabilities/AbstractVirtualTimeCapability.h>
#include <gloperate/capabilities/AbstractTargetFramebufferCapability.h>

#include <gloperate/capabilities/ViewportCapability.h>
#include <gloperate/capabilities/VirtualTimeCapability.h>
#include <gloperate/capabilities/TargetFramebufferCapability.h>

#include <gloperate/util/AdaptiveGrid.h>
#include <gloperate/util/Icosahedron.h>
#include <globjects/base/StringTemplate.h>
#include <gloperate/util/ScreenAlignedQuad.h>


class RasterizationStage : public gloperate::AbstractStage
{
public:
    RasterizationStage()
    : AbstractStage("Rasterization")
    {
        addInput("viewport", viewport);
        addInput("camera", camera);
        addInput("time", time);

        addOutput("color", color);
        addOutput("normal", normal);
        addOutput("geometry", geometry);
    }

    virtual ~RasterizationStage()
    {
    }

    virtual void initialize() override
    {
        m_fbo = new globjects::Framebuffer;

        color.data() = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
        normal.data() = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
        geometry.data() = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
        m_depth = new globjects::Renderbuffer();

        m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, color.data());
        m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT1, normal.data());
        m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT2, geometry.data());
        m_fbo->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, m_depth);

        globjects::StringTemplate* sphereVertexShader = new globjects::StringTemplate(new globjects::File("data/postprocessing/sphere.vert"));
        globjects::StringTemplate* sphereFragmentShader = new globjects::StringTemplate(new globjects::File("data/postprocessing/sphere.frag"));

        #ifdef __APPLE__
            sphereVertexShader->replace("#version 140", "#version 150");
            sphereFragmentShader->replace("#version 140", "#version 150");
        #endif

        m_program = new globjects::Program;

        m_program->attach(
            new globjects::Shader(gl::GL_VERTEX_SHADER, sphereVertexShader),
            new globjects::Shader(gl::GL_FRAGMENT_SHADER, sphereFragmentShader)
        );

        m_icosahedron = new gloperate::Icosahedron(2);
    }

    gloperate::InputSlot<gloperate::AbstractViewportCapability *> viewport;
    gloperate::InputSlot<gloperate::AbstractVirtualTimeCapability *> time;
    gloperate::InputSlot<gloperate::Camera *> camera;

    gloperate::Data<globjects::ref_ptr<globjects::Texture>> color;
    gloperate::Data<globjects::ref_ptr<globjects::Texture>> normal;
    gloperate::Data<globjects::ref_ptr<globjects::Texture>> geometry;
protected:
    virtual void process() override
    {
        camera.data()->setAspectRatio(glm::ivec2(viewport.data()->x(), viewport.data()->y()));
        gl::glViewport(viewport.data()->x(), viewport.data()->y(), viewport.data()->width(), viewport.data()->height());

        color.data()->image2D(0, gl::GL_RGBA32F, viewport.data()->width(), viewport.data()->height(), 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
        normal.data()->image2D(0, gl::GL_RGBA32F, viewport.data()->width(), viewport.data()->height(), 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
        geometry.data()->image2D(0, gl::GL_RGBA32F, viewport.data()->width(), viewport.data()->height(), 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);

        m_depth->storage(gl::GL_DEPTH_COMPONENT32, viewport.data()->width(), viewport.data()->height());

        m_program->setUniform("transform", camera.data()->viewProjection());
        m_program->setUniform("timef", time.data()->time());

        m_fbo->bind(gl::GL_FRAMEBUFFER);
        m_fbo->setDrawBuffers({ gl::GL_COLOR_ATTACHMENT0, gl::GL_COLOR_ATTACHMENT1, gl::GL_COLOR_ATTACHMENT2 });

        gl::glClearColor(1.0, 1.0, 1.0, 0.0);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        m_program->use();
        m_icosahedron->draw();
        m_program->release();

        m_fbo->unbind(gl::GL_FRAMEBUFFER);
    }
protected:
    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
    globjects::ref_ptr<globjects::Renderbuffer> m_depth;
    globjects::ref_ptr<globjects::Program> m_program;
    globjects::ref_ptr<gloperate::Icosahedron> m_icosahedron;
};

class PostprocessingStage : public gloperate::AbstractStage
{
public:
    PostprocessingStage()
    : AbstractStage("Postprocessing")
    {
        addInput("targetFBO", targetFramebuffer);
        addInput("color", color);
        addInput("normal", normal);
        addInput("geometry", geometry);

        alwaysProcess(true);
    }

    virtual ~PostprocessingStage()
    {
    }

    virtual void initialize() override
    {
        globjects::StringTemplate* phongVertexShader = new globjects::StringTemplate(new globjects::File("data/postprocessing/phong.vert"));
        globjects::StringTemplate* phongFragmentShader = new globjects::StringTemplate(new globjects::File("data/postprocessing/phong.frag"));

        #ifdef __APPLE__
            phongVertexShader->replace("#version 140", "#version 150");
            phongFragmentShader->replace("#version 140", "#version 150");
        #endif

        globjects::Program * program = new globjects::Program;

        program->attach(
            new globjects::Shader(gl::GL_VERTEX_SHADER, phongVertexShader),
            new globjects::Shader(gl::GL_FRAGMENT_SHADER, phongFragmentShader)
        );

        program->setUniform("color", 0);
        program->setUniform("normal", 1);
        program->setUniform("geometry", 2);

        m_quad = new gloperate::ScreenAlignedQuad(program);
    }

    gloperate::InputSlot<gloperate::AbstractTargetFramebufferCapability * > targetFramebuffer;

    gloperate::InputSlot<globjects::ref_ptr<globjects::Texture>> color;
    gloperate::InputSlot<globjects::ref_ptr<globjects::Texture>> normal;
    gloperate::InputSlot<globjects::ref_ptr<globjects::Texture>> geometry;
protected:
    virtual void process() override
    {
        gl::glDisable(gl::GL_DEPTH_TEST);

        color.data()->bindActive(gl::GL_TEXTURE0);
        normal.data()->bindActive(gl::GL_TEXTURE1);
        geometry.data()->bindActive(gl::GL_TEXTURE2);

        globjects::Framebuffer * fbo = targetFramebuffer.data()->framebuffer() ? targetFramebuffer.data()->framebuffer() : globjects::Framebuffer::defaultFBO();

        fbo->bind(gl::GL_FRAMEBUFFER);

        if (!fbo->isDefault())
        {
            fbo->setDrawBuffers({ gl::GL_COLOR_ATTACHMENT0 });
        }

        gl::glClearColor(1.0, 1.0, 1.0, 0.0);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        m_quad->draw();

        color.data()->unbindActive(gl::GL_TEXTURE0);
        normal.data()->unbindActive(gl::GL_TEXTURE1);
        geometry.data()->unbindActive(gl::GL_TEXTURE2);

        globjects::Framebuffer::defaultFBO()->bind(gl::GL_FRAMEBUFFER);

        gl::glEnable(gl::GL_DEPTH_TEST);
    }
protected:
    globjects::ref_ptr<gloperate::ScreenAlignedQuad> m_quad;
};

PostprocessingPipeline::PostprocessingPipeline()
: m_rasterization(new RasterizationStage)
, m_postprocessing(new PostprocessingStage)
, m_camera(new gloperate::Camera(glm::vec3(0.f, 1.f, 4.0f)))
, m_targetFBO(new gloperate::TargetFramebufferCapability)
, m_viewport(new gloperate::ViewportCapability)
, m_time(new gloperate::VirtualTimeCapability)
{
    m_targetFBO.data()->changed.connect([this]() {
        m_targetFBO.invalidate();
    });
    m_viewport.data()->changed.connect([this]() {
        m_viewport.invalidate();
    });
    m_time.data()->changed.connect([this]() {
        m_time.invalidate();
    });

    m_camera.data()->setZNear(0.1f);
    m_camera.data()->setZFar(16.f);

    m_time.data()->setLoopDuration(glm::pi<float>() * 2);

    m_rasterization->camera = m_camera;
    m_rasterization->viewport = m_viewport;
    m_rasterization->time = m_time;

    m_postprocessing->color = m_rasterization->color;
    m_postprocessing->normal = m_rasterization->normal;
    m_postprocessing->geometry = m_rasterization->geometry;
    m_postprocessing->targetFramebuffer = m_targetFBO;

    addStages(
        m_rasterization,
        m_postprocessing
    );
}

PostprocessingPipeline::~PostprocessingPipeline()
{
    delete m_camera.data();
}

gloperate::AbstractTargetFramebufferCapability * PostprocessingPipeline::targetFramebufferCapability()
{
    return m_targetFBO;
}

gloperate::AbstractViewportCapability * PostprocessingPipeline::viewportCapability()
{
    return m_viewport;
}

gloperate::AbstractVirtualTimeCapability * PostprocessingPipeline::virtualTimeCapability()
{
    return m_time;
}
