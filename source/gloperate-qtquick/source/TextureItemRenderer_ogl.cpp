
#include <gloperate-qtquick/TextureItemRenderer.h>

#include <cppassist/memory/make_unique.h>

#include <glbinding/gl/gl.h>

#include <globjects/base/AbstractStringSource.h>
#include <globjects/Framebuffer.h>
#include <globjects/FramebufferAttachment.h>
#include <globjects/AttachedTexture.h>
#include <globjects/Texture.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/Canvas.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/rendering/ScreenAlignedQuad.h>
#include <gloperate/rendering/ColorRenderTarget.h>

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
    Stage * stage = canvas->renderStage();
    if (!stage) return;

    std::string slotPath = m_path.toStdString();

    // Replace root in slot path with render stage name
    if (string::hasPrefix(slotPath, "root")) {
        slotPath.replace(0, 4, stage->name());
    }

    AbstractSlot * slot = stage->getSlot(slotPath);
    if (!slot) return;

    // Check if it is a texture slot
    if (slot && slot->type() == typeid(globjects::Texture *))
    {
        texture = static_cast< Slot<globjects::Texture *> * >(slot)->value();
    }

    // Check if it is a texture slot
    if (slot && slot->type() == typeid(gloperate::ColorRenderTarget *))
    {
        gloperate::ColorRenderTarget * renderTarget = static_cast< Slot<gloperate::ColorRenderTarget *> * >(slot)->value();
        if (renderTarget->currentTargetType() == RenderTargetType::UserDefinedFBOAttachment)
        {
            texture = renderTarget->framebufferAttachment()->asTextureAttachment()->texture();
        }
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

    // Bind default FBO
    m_fbo->bind(gl::GL_FRAMEBUFFER);

    // Set viewport
    gl::glViewport(0, 0, m_width, m_height);

    // Disable depth test for screen-aligned quad
    gl::glDisable(gl::GL_DEPTH_TEST);

    // Enable blending
    gl::glEnable(gl::GL_BLEND);

    // Draw screen-aligned quad
    m_screenAlignedQuad->setTexture(texture);
    m_screenAlignedQuad->draw();

    // Unbind texture
    texture->unbind();

    // Restore OpenGL states
    gl::glEnable(gl::GL_DEPTH_TEST);
}

void TextureItemRenderer::buildGeometry()
{
    // Create screen-aligned quad
    m_screenAlignedQuad = cppassist::make_unique<gloperate::ScreenAlignedQuad>();
    m_screenAlignedQuad->setInverted(true);
}


} // namespace gloperate_qtquick
