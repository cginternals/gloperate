
#include "ScreenAligned.h"

#include <random>

#include <glbinding/gl/gl.h>

#include <globjects/logging.h>

#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/TargetFramebufferCapability.h>


using namespace gloperate;
using namespace globjects;
using namespace gl;


ScreenAligned::ScreenAligned(ResourceManager & resourceManager, const cpplocate::ModuleInfo & moduleInfo)
: Painter("ScreenAligned", resourceManager, moduleInfo)
{
    // Setup painter
    m_targetFramebufferCapability = addCapability(new gloperate::TargetFramebufferCapability());
    m_viewportCapability = addCapability(new gloperate::ViewportCapability());
}

ScreenAligned::~ScreenAligned()
{
}

void ScreenAligned::onInitialize()
{
#ifdef __APPLE__
    Shader::clearGlobalReplacements();
    Shader::globalReplace("#version 140", "#version 150");

    debug() << "Using global OS X shader replacement '#version 140' -> '#version 150'" << std::endl;
#endif

    gl::glClearColor(0.2f, 0.3f, 0.4f, 1.f);

    createAndSetupTexture();
    createAndSetupGeometry();
}

void ScreenAligned::onPaint()
{
    if (m_viewportCapability->hasChanged())
    {
        glViewport(m_viewportCapability->x(), m_viewportCapability->y(), m_viewportCapability->width(), m_viewportCapability->height());

        m_viewportCapability->setChanged(false);
    }

    globjects::Framebuffer * fbo = m_targetFramebufferCapability->framebuffer();

    if (!fbo)
    {
        fbo = globjects::Framebuffer::defaultFBO();
    }

    fbo->bind(GL_FRAMEBUFFER);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_quad->draw();

    globjects::Framebuffer::unbind(GL_FRAMEBUFFER);
}

void ScreenAligned::createAndSetupTexture()
{
    static const int w(256);
    static const int h(256);
    unsigned char data[w * h * 4];

    std::random_device rd;
    std::mt19937 generator(rd());
    std::poisson_distribution<> r(0.2);

    for (int i = 0; i < w * h * 4; ++i) {
        data[i] = static_cast<unsigned char>(255 - static_cast<unsigned char>(r(generator) * 255));
    }

    m_texture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
    m_texture->image2D(0, gl::GL_RGBA8, w, h, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, data);
}

void ScreenAligned::createAndSetupGeometry()
{
    m_quad = new gloperate::ScreenAlignedQuad(m_texture);
    m_quad->setSamplerUniform(0);
}
