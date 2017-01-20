
#include <gloperate/stages/base/FramebufferStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/Texture.h>

#include <gloperate/rendering/RenderTarget.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(FramebufferStage, gloperate::Stage)


FramebufferStage::FramebufferStage(Environment * environment, const std::string & name)
: Stage(environment, "FramebufferStage", name)
, colorTexture("colorTexture", this)
, depthTexture("depthTexture", this)
, fbo         ("fbo", this)
{
}

FramebufferStage::~FramebufferStage()
{
}

void FramebufferStage::onContextInit(AbstractGLContext *)
{
}

void FramebufferStage::onContextDeinit(AbstractGLContext *)
{
}

void FramebufferStage::onProcess(AbstractGLContext *)
{
    // Check if FBO needs to be rebuilt
    if (!fbo.isValid())
    {
        // Rebuild FBO (and textures)
        rebuildFBO();

        // Update outputs
        this->fbo.setValue(m_fbo);
    }
}

void FramebufferStage::rebuildFBO()
{
    // Create FBO
    m_fbo = new globjects::Framebuffer;

    // Attach textures to FBO
    std::vector<gl::GLenum> colorAttachments;
    for (int i = 0; i <= 1; i++) {
        // First round: Count number of color attachments
        // Second round: Actually attach the textures
        gl::GLenum index = gl::GL_COLOR_ATTACHMENT0;
        if (i == 1) {
            m_fbo->setDrawBuffers(colorAttachments);
        }

        for (auto input : this->inputs()) {
            auto slot = dynamic_cast<Input<RenderTarget *> *>(input);
            if (!slot) {
                continue;
            }
            RenderTarget * texture = **slot;
            if (!texture)
            {
                continue;
            }

            if (i == 0) {
                if (!isNameOfDepthRenderTarget(slot->name())) {
                    // Add color attachment
                    colorAttachments.push_back(index);
                    index = index + 1;
                }
            } else {
                if (isNameOfDepthRenderTarget(slot->name())) {
                    // Add depth attachment
                    texture->bind(gl::GL_DEPTH_ATTACHMENT, m_fbo);
                } else {
                    // Add color attachment
                    texture->bind(index, m_fbo);
                    index = index + 1;
                }
            }
        }
    }
}

bool FramebufferStage::isNameOfDepthRenderTarget(const std::string & name)
{
    return name.find("Depth") != std::string::npos || name.find("depth") != std::string::npos;
}



} // namespace gloperate
