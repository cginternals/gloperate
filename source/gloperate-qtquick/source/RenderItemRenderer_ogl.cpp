
#include <gloperate-qtquick/RenderItemRenderer.h>

#include <glm/vec2.hpp>

#include <cppassist/memory/make_unique.h>

#include <glbinding/gl/gl.h>

#include <globjects/base/AbstractStringSource.h>
#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>

#include <gloperate/rendering/ScreenAlignedQuad.h>
#include <gloperate-qt/base/GLContext.h>

#include <gloperate-qtquick/RenderItem.h>
#include <gloperate/base/Canvas.h>


namespace gloperate_qtquick
{


RenderItemRenderer::RenderItemRenderer(RenderItem * renderItem)
: m_renderItem(renderItem)
, m_contextInitialized(false)
, m_canvasInitialized(false)
, m_width(0)
, m_height(0)
{
}

RenderItemRenderer::~RenderItemRenderer()
{
    // free pipeline as the pipeline got initialized using OpenGL context of this
    // [TODO]: Reevaluate the destruction / deinitialization chain
    m_renderItem->canvas()->onContextDeinit();
}

void RenderItemRenderer::configureFbo(int fboId, unsigned int width, unsigned int height)
{
    // Create wrapper for the outer FBO
    m_fbo = globjects::Framebuffer::fromId(fboId);

    // Save FBO size
    m_width  = width;
    m_height = height;

    // Set texture options
    glm::ivec2 size = glm::ivec2(width, height);
    gl::GLenum format         = gl::GL_RGBA;
    gl::GLenum internalFormat = gl::GL_RGBA;
    gl::GLenum dataType       = gl::GL_UNSIGNED_BYTE;

    // Resize color texture
    m_texColor->image2D(0, internalFormat, size.x, size.y, 0, format, dataType, nullptr);

    // Resize depth texture
    m_texDepth->image2D(0, gl::GL_DEPTH_COMPONENT, size.x, size.y, 0, gl::GL_DEPTH_COMPONENT, gl::GL_UNSIGNED_BYTE, nullptr);

    // Create FBO
    m_innerFbo = cppassist::make_unique<globjects::Framebuffer>();
    m_innerFbo->setDrawBuffers({ gl::GL_COLOR_ATTACHMENT0 });
    m_innerFbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_texColor.get());
    m_innerFbo->attachTexture(gl::GL_DEPTH_ATTACHMENT,  m_texDepth.get());
}

void RenderItemRenderer::initializeFboAttachments()
{
    // Create color texture
    m_texColor = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);

    // Create depth texture
    m_texDepth = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);

    // Create screen-aligned quad
    m_screenAlignedQuad = cppassist::make_unique<gloperate::ScreenAlignedQuad>();

    // Create shader program
    m_vertexShaderSource   = m_screenAlignedQuad->vertexShaderSource();
    m_fragmentShaderSource = m_screenAlignedQuad->fragmentShaderSourceInverted();

    m_vertexShader   = cppassist::make_unique<globjects::Shader>(gl::GL_VERTEX_SHADER,   m_vertexShaderSource.get());
    m_fragmentShader = cppassist::make_unique<globjects::Shader>(gl::GL_FRAGMENT_SHADER, m_fragmentShaderSource.get());

    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());
    m_program->setUniform("source", 0);
}

void RenderItemRenderer::renderTexture()
{
    m_fbo->bind(gl::GL_FRAMEBUFFER);
    gl::glViewport(0, 0, m_width, m_height);
    gl::glDisable(gl::GL_DEPTH_TEST);
    gl::glEnable(gl::GL_BLEND);
    gl::glDisable(gl::GL_CULL_FACE);

    gl::glActiveTexture(gl::GL_TEXTURE0);
    m_texColor->bind();

    m_program->use();
    m_screenAlignedQuad->draw();
    m_program->release();
}


} // namespace gloperate_qtquick
