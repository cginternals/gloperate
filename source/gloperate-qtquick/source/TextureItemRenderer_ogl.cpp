
#include <gloperate-qtquick/TextureItemRenderer.h>

#include <cppassist/memory/make_unique.h>

#include <glbinding/gl/gl.h>

#include <globjects/base/AbstractStringSource.h>
#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/Canvas.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/rendering/ScreenAlignedQuad.h>

#include <gloperate-qtquick/TextureItem.h>


using namespace gloperate;


namespace gloperate_qtquick
{


TextureItemRenderer::TextureItemRenderer(TextureItem * textureItem, gloperate::Environment * environment)
: m_textureItem(textureItem)
, m_environment(environment)
, m_width(0)
, m_height(0)
{
}

TextureItemRenderer::~TextureItemRenderer()
{
}

void TextureItemRenderer::configureFbo(int fboId, unsigned int width, unsigned int height)
{
    // Create wrapper for the outer FBO
    m_fbo = globjects::Framebuffer::fromId(fboId);

    // Save FBO size
    m_width  = width;
    m_height = height;
}

void TextureItemRenderer::renderTexture()
{
    if (m_environment->canvases().empty())
    {
        return;
    }

    // Get texture
    globjects::Texture * texture = nullptr;

    // Get slot
    Canvas * canvas = m_environment->canvases().front();
    AbstractSlot * slot = canvas->renderStage()->output(m_path.toStdString());
    if (!slot) return;

    // Check if it is a texture slot
    if (slot && slot->type() == typeid(globjects::Texture *))
    {
        texture = static_cast< Slot<globjects::Texture *> * >(slot)->value();
    }

    // Abort if texture is invalid
    if (!texture)
    {
        return;
    }

    // Check if geometry needs to be built
    if (!m_screenAlignedQuad.get())
    {
        buildGeometry();
    }

    // Check if program needs to be (re-)built
    if (!m_program.get())
    {
        buildProgram();
    }

    // Bind default FBO
    m_fbo->bind(gl::GL_FRAMEBUFFER);

    // Set viewport
    gl::glViewport(0, 0, m_width, m_height);

    // Disable depth test for screen-aligned quad
    gl::glDisable(gl::GL_DEPTH_TEST);

    // Enable blending
    gl::glEnable(gl::GL_BLEND);

    // Bind texture
    texture->bindActive(0);

    // Draw screen-aligned quad
    m_program->use();
    m_screenAlignedQuad->draw();
    m_program->release();

    // Unbind texture
    texture->unbind();

    // Restore OpenGL states
    gl::glEnable(gl::GL_DEPTH_TEST);
}

void TextureItemRenderer::buildGeometry()
{
    // Create screen-aligned quad
    m_screenAlignedQuad = cppassist::make_unique<gloperate::ScreenAlignedQuad>();
}

void TextureItemRenderer::buildProgram()
{
    // Create shaders
    m_vertexShaderSource   = m_screenAlignedQuad->vertexShaderSource();
    m_fragmentShaderSource = m_screenAlignedQuad->fragmentShaderSourceInverted();

    m_vertexShader   = cppassist::make_unique<globjects::Shader>(gl::GL_VERTEX_SHADER,   m_vertexShaderSource.get());
    m_fragmentShader = cppassist::make_unique<globjects::Shader>(gl::GL_FRAGMENT_SHADER, m_fragmentShaderSource.get());

    // Create program
    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());
    m_program->setUniform("source", 0);
}


} // namespace gloperate_qtquick
