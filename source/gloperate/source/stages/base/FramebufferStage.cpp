
#include <gloperate/stages/base/FramebufferStage.h>

#include <glbinding/gl/gl.h>


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
    // [TODO] Iterate over dynamic inputs
    m_fbo->setDrawBuffers({ gl::GL_COLOR_ATTACHMENT0 });
    m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, *colorTexture);
    m_fbo->attachTexture(gl::GL_DEPTH_ATTACHMENT,  *depthTexture);

/*
    // Attach textures to FBO
    std::vector<gl::GLenum> colorAttachments;
    for (int i=0; i<=1; i++) {
        // First round: Count number of color attachments
        // Second round: Actually attach the textures
        gl::GLenum index = gl::GL_COLOR_ATTACHMENT0;
        if (i == 1) {
            m_fbo->setDrawBuffers(colorAttachments);
        }

        for (auto input : this->inputs()) {
            if (auto slot = dynamic_cast<gloperate::InputSlot<globjects::Texture *> *>(input)) {
                // Get texture (if set)
                globjects::Texture * texture = slot->data();
                if (texture && i == 0) {
                    if (slot->name() != "texDepth") {
                        // Add color attachment
                        colorAttachments.push_back(index);
                        index = index + 1;
                    }
                } else if (texture && i == 1) {
                    if (slot->name() == "texDepth") {
                        // Add depth attachment
                        m_fbo->attachTexture(gl::GL_DEPTH_ATTACHMENT, texture);
                    } else {
                        // Add color attachment
                        m_fbo->attachTexture(index, texture);
                        index = index + 1;
                    }
                }
            }
        }
    }
*/
}


} // namespace gloperate
