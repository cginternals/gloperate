
#include <gloperate-glheadless/stages/demos/DemoOffscreenStage.h>

#include <globjects/globjects.h>
#include <globjects/Texture.h>

#include <gloperate/base/AbstractGLContext.h>
#include <gloperate/stages/demos/DemoStage.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>

#include <gloperate-glheadless/base/GLContext.h>
#include <gloperate-glheadless/base/GLContextFactory.h>
#include <gloperate-glheadless/util/ScopedContext.h>
#include <glbinding/gl/enum.h>


using namespace gloperate;


namespace gloperate_glheadless
{


CPPEXPOSE_COMPONENT(DemoOffscreenStage, Stage)


DemoOffscreenStage::DemoOffscreenStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, viewport("viewport", this)
, backgroundColor("backgroundColor", this)
, timeDelta("timeDelta", this)
, colorTexture("colorTexture", this)
, m_renderStage(new DemoStage(environment))
{
    // set up target render stage
    m_renderStage->renderInterface.deviceViewport  << viewport;
    m_renderStage->renderInterface.backgroundColor << backgroundColor;
    m_renderStage->renderInterface.timeDelta       << timeDelta;
}


DemoOffscreenStage::~DemoOffscreenStage()
{
}


void DemoOffscreenStage::onContextInit(AbstractGLContext * context)
{
    globjects::init();

    createAndSetUpOffscreenContext(context);
}


void DemoOffscreenStage::onContextDeinit(AbstractGLContext * context)
{
    m_offscreenContext = nullptr;
    m_framebuffer = nullptr;
    m_colorTexture = nullptr;
    m_depthTexture = nullptr;
}


void DemoOffscreenStage::onProcess(AbstractGLContext * context)
{
    // Activate offscreen context
    ScopedContext contextSwitch(context, m_offscreenContext.get());

    // Check if FBO needs to be rebuilt
    if (!m_framebuffer)
    {
        // Rebuild FBO (and textures)
        rebuildFBO();

        // Update outputs
        colorTexture.setValue(m_colorTexture);
        m_renderStage->renderInterface.targetFBO.setValue(m_framebuffer);
    }

    // Execute target stage
    m_renderStage->process(context);
}


void DemoOffscreenStage::createAndSetUpOffscreenContext(AbstractGLContext * context)
{
    // Create context format
    GLContextFormat format;
    format.setVersion(3, 2);
    format.setProfile(GLContextFormat::Profile::Core);

    // Create offscreen context
    GLContextFactory factory;
    factory.setSharedContext(context);
    m_offscreenContext.reset(factory.createBestContext(format));

    // Activate offscreen context & init target stage
    ScopedContext contextSwitch(context, m_offscreenContext.get());
    m_renderStage->initContext(context);
}


void DemoOffscreenStage::rebuildFBO() {
    // Get texture size
    glm::ivec2 size = glm::ivec2((*this->viewport).z, (*this->viewport).w);

    // Create color texture
    m_colorTexture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
    m_colorTexture->image2D(0, gl::GL_RGBA, size.x, size.y, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);

    // Create depth texture
    m_depthTexture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
    m_depthTexture->image2D(0, gl::GL_DEPTH_COMPONENT, size.x, size.y, 0, gl::GL_DEPTH_COMPONENT, gl::GL_UNSIGNED_BYTE, nullptr);

    // Create FBO
    m_framebuffer = new globjects::Framebuffer;
    m_framebuffer->setDrawBuffers({ gl::GL_COLOR_ATTACHMENT0 });
    m_framebuffer->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_colorTexture);
    m_framebuffer->attachTexture(gl::GL_DEPTH_ATTACHMENT, m_depthTexture);
}


} // namespace gloperate_glheadless
