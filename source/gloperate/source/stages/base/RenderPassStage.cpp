
#include <gloperate/stages/base/RenderPassStage.h>

#include <glbinding/gl/enum.h>

#include <globjects/Program.h>
#include <globjects/State.h>
#include <globjects/Texture.h>

#include <gloperate/base/Color.h>

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
    m_inputAddedConnection = inputAdded.connect([this] (gloperate::AbstractSlot *) {
        renderPass.invalidate();
    });
    m_inputRemovedConnection = inputRemoved.connect([this] (gloperate::AbstractSlot *) {
        renderPass.invalidate();
    });
}

RenderPassStage::~RenderPassStage()
{
}

void RenderPassStage::onContextInit(AbstractGLContext *)
{
    m_renderPass = cppassist::make_unique<gloperate::RenderPass>();
    renderPass.setValue(m_renderPass.get());

    m_beforeState = cppassist::make_unique<globjects::State>(globjects::State::DeferredMode);

    m_renderPass->setStateBefore(m_beforeState.get());

    m_renderPass->stateBefore()->enable(gl::GL_DEPTH_TEST);
    m_renderPass->stateBefore()->enable(gl::GL_CULL_FACE);
    m_renderPass->stateBefore()->depthFunc(gl::GL_LEQUAL);
    m_renderPass->stateBefore()->disable(gl::GL_BLEND);
    m_renderPass->stateBefore()->cullFace(gl::GL_BACK);
    m_renderPass->stateBefore()->frontFace(gl::GL_CCW);
    m_renderPass->stateBefore()->depthMask(gl::GL_TRUE);
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

    for (auto input : inputs<globjects::Texture *>()) {
        (*program)->setUniform<int>(input->name(), textureIndex);
        m_renderPass->setTexture(textureIndex, **input);

        if ((**input)->target() == gl::GL_TEXTURE_CUBE_MAP)
        {
            (*renderPass)->stateBefore()->enable(gl::GL_TEXTURE_CUBE_MAP_SEAMLESS);
        }

        ++textureIndex;
    }
    for (auto bufferInput : inputs<globjects::Buffer *>()) {
        m_renderPass->setShaderStorageBuffer(shaderStorageBufferIndex, **bufferInput);

        ++shaderStorageBufferIndex;
    }

    for (auto input : inputs<Color>()) {
        (*program)->setUniform<glm::vec4>(input->name(), (*input)->toVec4());
    }

    for (auto setterPair : uniformSetters)
    {
        setterPair.second();
    }

    renderPass.setValue(m_renderPass.get());
}


} // namespace gloperate
