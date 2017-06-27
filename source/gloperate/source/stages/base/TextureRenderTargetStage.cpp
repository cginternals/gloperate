
#include <gloperate/stages/base/TextureRenderTargetStage.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>

#include <gloperate/rendering/ColorRenderTarget.h>
#include <gloperate/rendering/DepthRenderTarget.h>
#include <gloperate/rendering/StencilRenderTarget.h>


using namespace gl;
using namespace globjects;


namespace gloperate
{


CPPEXPOSE_COMPONENT(TextureRenderTargetStage, gloperate::Stage)


TextureRenderTargetStage::TextureRenderTargetStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "TextureRenderTargetStage", name)
, internalFormat("internalFormat", this)
, format("format", this)
, type("type", this)
, size("size", this)
, texture("texture", this)
, colorRenderTarget("colorRenderTarget", this)
, depthRenderTarget("depthRenderTarget", this)
, stencilRenderTarget("stencilRenderTarget", this)
{
}

TextureRenderTargetStage::~TextureRenderTargetStage()
{
}

void TextureRenderTargetStage::onContextInit(gloperate::AbstractGLContext *)
{
    // Create new texture
    m_texture = Texture::createDefault(GL_TEXTURE_2D);

    // Create wrapping render target
    m_colorRenderTarget   = cppassist::make_unique<ColorRenderTarget>();
    m_depthRenderTarget   = cppassist::make_unique<DepthRenderTarget>();
    m_stencilRenderTarget = cppassist::make_unique<StencilRenderTarget>();

    m_colorRenderTarget->setAttachmentType(AttachmentType::Color);
    m_stencilRenderTarget->setAttachmentType(AttachmentType::DepthStencil);
}

void TextureRenderTargetStage::onContextDeinit(AbstractGLContext *)
{
    // Clean up OpenGL objects
    m_texture             = nullptr;
    m_colorRenderTarget   = nullptr;
    m_depthRenderTarget   = nullptr;
    m_stencilRenderTarget = nullptr;
}

void TextureRenderTargetStage::onProcess()
{
    // Check if texture has been created successfully
    if (!m_texture.get())
    {
        return;
    }

    // Create texture image
    const auto width  = (*size)[2];
    const auto height = (*size)[3];
    m_texture->image2D(0, *internalFormat, width, height, 0, *format, *type, nullptr);

    switch(*internalFormat)
    {
    case GL_DEPTH_COMPONENT:
    case GL_DEPTH_COMPONENT16:
    case GL_DEPTH_COMPONENT24:
    case GL_DEPTH_COMPONENT32F:
        m_colorRenderTarget->releaseTarget();
        m_stencilRenderTarget->releaseTarget();

        m_depthRenderTarget->setTarget(m_texture.get());

        m_depthRenderTarget->setAttachmentType(AttachmentType::Depth);
        break;
    case GL_DEPTH_STENCIL:
    case GL_DEPTH24_STENCIL8:
    case GL_DEPTH32F_STENCIL8:
        m_colorRenderTarget->releaseTarget();

        m_depthRenderTarget->setTarget(m_texture.get());
        m_stencilRenderTarget->setTarget(m_texture.get());

        m_depthRenderTarget->setAttachmentType(AttachmentType::DepthStencil);
        break;
    default: // Color attachment
        m_depthRenderTarget->releaseTarget();
        m_stencilRenderTarget->releaseTarget();

        m_colorRenderTarget->setTarget(m_texture.get());
        break;
    }

    // Update outputs
    texture.setValue(m_texture.get());
    colorRenderTarget.setValue(m_colorRenderTarget.get());
    depthRenderTarget.setValue(m_depthRenderTarget.get());
    stencilRenderTarget.setValue(m_stencilRenderTarget.get());
}


} // namespace gloperate
