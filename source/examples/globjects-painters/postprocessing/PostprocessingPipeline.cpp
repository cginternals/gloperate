
#include "PostprocessingPipeline.h"

#include <glm/gtc/constants.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/base/File.h>
#include <globjects/base/StringTemplate.h>
#include <globjects/logging.h>
#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>
#include <globjects/Renderbuffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>

#include <gloperate/base/RenderTargetType.h>
#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/Data.h>
#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/painter/Camera.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractVirtualTimeCapability.h>
#include <gloperate/painter/AbstractTargetFramebufferCapability.h>
#include <gloperate/painter/AbstractProjectionCapability.h>
#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/PerspectiveProjectionCapability.h>
#include <gloperate/painter/TypedRenderTargetCapability.h>
#include <gloperate/primitives/AdaptiveGrid.h>
#include <gloperate/primitives/Icosahedron.h>
#include <gloperate/primitives/ScreenAlignedQuad.h>


class RasterizationStage : public gloperate::AbstractStage
{
public:
    RasterizationStage()
    : AbstractStage("Rasterization")
    {
        addInput("viewport", viewport);
        addInput("camera", camera);
        addInput("projection", projection);
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

    globjects::Framebuffer * fbo()
    {
        return m_fbo;
    }


public:
    gloperate::InputSlot<gloperate::AbstractViewportCapability *> viewport;
    gloperate::InputSlot<gloperate::AbstractVirtualTimeCapability *> time;
    gloperate::InputSlot<gloperate::AbstractCameraCapability *> camera;
    gloperate::InputSlot<gloperate::AbstractProjectionCapability *> projection;

    gloperate::Data<globjects::ref_ptr<globjects::Texture>> color;
    gloperate::Data<globjects::ref_ptr<globjects::Texture>> normal;
    gloperate::Data<globjects::ref_ptr<globjects::Texture>> geometry;


protected:
    virtual void process() override
    {
        gl::glViewport(viewport.data()->x(), viewport.data()->y(), viewport.data()->width(), viewport.data()->height());

        color.data()->image2D(0, gl::GL_RGBA32F, viewport.data()->width(), viewport.data()->height(), 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
        normal.data()->image2D(0, gl::GL_RGBA32F, viewport.data()->width(), viewport.data()->height(), 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
        geometry.data()->image2D(0, gl::GL_RGBA32F, viewport.data()->width(), viewport.data()->height(), 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);

        m_depth->storage(gl::GL_DEPTH_COMPONENT32, viewport.data()->width(), viewport.data()->height());

        m_program->setUniform("transform", projection.data()->projection() * camera.data()->view());
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


public:
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
{
    auto rasterizationStage = new RasterizationStage();
    auto postprocessingStage = new PostprocessingStage();

    rasterizationStage->camera = camera;
    rasterizationStage->viewport = viewport;
    rasterizationStage->time = time;
    rasterizationStage->projection = projection;

    postprocessingStage->color = rasterizationStage->color;
    postprocessingStage->normal = rasterizationStage->normal;
    postprocessingStage->geometry = rasterizationStage->geometry;
    postprocessingStage->targetFramebuffer = targetFBO;

    rasterizationStage->color.invalidated.connect(
        [this, &rasterizationStage]()
        {
            auto typedRenderTargetCapability = dynamic_cast<gloperate::TypedRenderTargetCapability *>(renderTargets.data());

            if (typedRenderTargetCapability != nullptr)
            {
                typedRenderTargetCapability->setRenderTarget(gloperate::RenderTargetType::Depth, rasterizationStage->fbo(), gl::GLenum::GL_DEPTH_ATTACHMENT, gl::GLenum::GL_DEPTH_COMPONENT);
            }
        });

    addStages(
        rasterizationStage,
        postprocessingStage
    );
}

PostprocessingPipeline::~PostprocessingPipeline()
{
}
