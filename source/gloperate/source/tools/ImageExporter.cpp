
#include <gloperate/tools/ImageExporter.h>

#include <cassert>

#include <glbinding/gl/gl.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/AbstractCanvas.h>
#include <gloperate/base/ResourceManager.h>
#include <gloperate/base/AbstractGLContext.h>

#include <globjects/Framebuffer.h>


namespace gloperate
{


ImageExporter::ImageExporter()
: m_canvas(nullptr)
, m_filename("")
, m_width(0)
, m_height(0)
, m_renderIterations(0)
{
}

ImageExporter::~ImageExporter()
{
}

void ImageExporter::setTarget(AbstractCanvas * canvas, const std::string & filename, int width, int height, int renderIterations)
{
    // Save configuration
    m_canvas           = canvas;
    m_filename         = filename;
    m_width            = width;
    m_height           = height;
    m_renderIterations = renderIterations;
}

void ImageExporter::save(ImageExporter::ContextHandling contextHandling)
{
    // Create output textures
    m_color = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
    m_depth = new globjects::Renderbuffer();

    // Create output FBO
    m_fbo   = new globjects::Framebuffer();
    m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_color);
    m_fbo->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, m_depth);

    // Set viewport
    auto viewport = glm::vec4(0, 0, m_width, m_height);
    m_canvas->onSaveViewport();
    m_canvas->onViewport(viewport, viewport);

    // Activate context (if necessary)
    if (contextHandling == ActivateContext)
    {
        m_canvas->openGLContext()->use();
    }

    // Initialize textures
    m_color->image2D(0, gl::GL_RGBA, m_width, m_height, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);
    m_depth->storage(gl::GL_DEPTH_COMPONENT32, m_width, m_height);

    // Render image
    for (int i = 0; i < m_renderIterations; ++i)
    {
        m_canvas->environment()->update(1.f / 30.f);
        m_canvas->onRender(m_fbo);
    }

    // Release context (if necessary)
    if (contextHandling == ActivateContext)
    {
        m_canvas->openGLContext()->release();
    }

    // Save texture to image file
    m_canvas->environment()->resourceManager()->store<globjects::Texture>(m_filename, m_color);

    // Reset viewport
    m_canvas->onResetViewport();
}


} // namespace gloperate
