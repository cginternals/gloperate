
#include <gloperate/tools/ImageExporter.h>

#include <cassert>

#include <glbinding/gl/gl.h>

#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/base/RenderSurface.h>
#include <gloperate/base/ResourceManager.h>
#include <gloperate/base/AbstractGLContext.h>
#include <gloperate/base/Image.h>

#include <globjects/Framebuffer.h>


namespace gloperate
{


ImageExporter::ImageExporter(RenderSurface * surface)
: m_surface(surface)
, m_context(surface->viewerContext())
, m_glContext(surface->openGLContext())
, m_fbo(new globjects::Framebuffer())
, m_color(globjects::Texture::createDefault(gl::GL_TEXTURE_2D))
, m_depth(new globjects::Renderbuffer())
, m_filename()
, m_width(0)
, m_height(0)
, m_renderIterations(0)
{
    m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_color);
    m_fbo->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, m_depth);
}

void ImageExporter::init(const std::string & filename, int width, int height, int renderIterations)
{
    m_filename = filename;
    m_width = width;
    m_height = height;
    m_renderIterations = renderIterations;
}

void ImageExporter::save(bool glContextActive)
{
    auto deviceViewport = m_surface->deviceViewport();
    auto virtualViewport = m_surface->virtualViewport();
    auto vp = glm::vec4(0, 0, m_width, m_height);

    m_surface->onViewport(vp, vp);

    Image image(m_width, m_height, gl::GL_RGB, gl::GL_UNSIGNED_BYTE);

    if (!glContextActive)
    {
        m_glContext->use();
    }

    m_color->image2D(0, gl::GL_RGBA, m_width, m_height, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);
    m_depth->storage(gl::GL_DEPTH_COMPONENT32, m_width, m_height);

    for (int i = 0; i < m_renderIterations; ++i)
    {
        m_context->update(1.f/30.f);
        m_surface->onRender(m_fbo);
    }

    if (!glContextActive)
    {
        m_glContext->release();
    }

    m_context->resourceManager()->store<globjects::Texture>(m_filename, m_color);

    m_surface->onViewport(deviceViewport, virtualViewport);
}


} // namespace gloperate
