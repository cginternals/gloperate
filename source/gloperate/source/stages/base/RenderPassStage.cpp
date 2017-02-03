
#include <gloperate/stages/base/RenderPassStage.h>

#include <glbinding/gl/enum.h>

#include <globjects/Program.h>
#include <globjects/State.h>
 #include <globjects/Texture.h>

#include <gloperate/rendering/RenderPass.h>
#include <gloperate/rendering/Drawable.h>
#include <gloperate/rendering/Camera.h>

#include <gloperate/gloperate.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(RenderPassStage, gloperate::Stage)


RenderPassStage::RenderPassStage(Environment * environment, const std::string & name)
: Stage(environment, "RenderPassStage", name)
, drawable("drawable", this)
, program("program", this)
, camera("camera", this)
, renderPass("renderPass", this)
{
    inputAdded.connect([this] (gloperate::AbstractSlot *) {
        renderPass.setValid(false);
    });
    inputRemoved.connect([this] (gloperate::AbstractSlot *) {
        renderPass.setValid(false);
    });
}

RenderPassStage::~RenderPassStage()
{
}

void RenderPassStage::onContextInit(AbstractGLContext *)
{
    m_renderPass = new gloperate::RenderPass;
    renderPass.setValue(m_renderPass);

    m_renderPass->setStateBefore(new globjects::State(globjects::State::DeferredMode));

//    m_renderPass->state()->enable(gl::GL_DEPTH_TEST);
//    m_renderPass->state()->enable(gl::GL_CULL_FACE);
//    m_renderPass->state()->depthFunc(gl::GL_LEQUAL);
//    m_renderPass->state()->disable(gl::GL_BLEND);
//    m_renderPass->state()->cullFace(gl::GL_BACK);
//    m_renderPass->state()->frontFace(gl::GL_CCW);
//    m_renderPass->state()->depthMask(gl::GL_TRUE);
}

void RenderPassStage::onProcess(AbstractGLContext *)
{
    m_renderPass->setGeometry((*drawable));
    m_renderPass->setProgram((*program));

    if (camera.isValid() && *camera)
    {
        gloperate::Camera * cameraPtr = *camera;

        (*program)->setUniform<glm::vec3>("eye", cameraPtr->eye());
        (*program)->setUniform<glm::mat4>("viewProjection", cameraPtr->viewProjection());
        (*program)->setUniform<glm::mat4>("view", cameraPtr->view());
        (*program)->setUniform<glm::mat4>("projection", cameraPtr->projection());
        (*program)->setUniform<glm::mat3>("normalMatrix", cameraPtr->normal());
    }

    unsigned int textureIndex = 0;
    unsigned int shaderStorageBufferIndex = 0;

    for (auto input : inputs()) {
        auto textureInput = dynamic_cast<Input<globjects::Texture *> *>(input);
        if (textureInput)
        {
            (*program)->setUniform<int>(textureInput->name(), textureIndex);
            m_renderPass->setTexture(textureIndex, **textureInput);

            if ((**textureInput)->target() == gl::GL_TEXTURE_CUBE_MAP)
            {
                (*renderPass)->stateBefore()->enable(gl::GL_TEXTURE_CUBE_MAP_SEAMLESS);
            }

            ++textureIndex;
        }

        auto bufferInput = dynamic_cast<Input<globjects::Buffer *> *>(input);
        if (bufferInput)
        {
            m_renderPass->setShaderStorageBuffer(shaderStorageBufferIndex, **bufferInput);

            ++shaderStorageBufferIndex;
        }
    }

    for (auto setterPair : uniformSetters)
    {
        setterPair.second();
    }

    renderPass.setValid(true);
}


} // namespace gloperate
