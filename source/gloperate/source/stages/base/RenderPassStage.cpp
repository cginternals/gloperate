
#include <gloperate/stages/base/RenderPassStage.h>

#include <glbinding/gl/enum.h>

#include <globjects/Program.h>
#include <globjects/State.h>
#include <globjects/Texture.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Color.h>
#include <gloperate/rendering/RenderPass.h>
#include <gloperate/rendering/Drawable.h>
#include <gloperate/rendering/Camera.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(RenderPassStage, gloperate::Stage)


RenderPassStage::RenderPassStage(Environment * environment, const std::string & name)
: Stage(environment, "RenderPassStage", name)
, drawable("drawable", this)
, program("program", this)
, camera("camera", this)
, depthTest("depthTest", this, true)
, depthMask("depthMask", this, true)
, depthFunc("depthFunc", this, gl::GL_LEQUAL)
, culling("culling", this, true)
, cullFace("cullFace", this, gl::GL_BACK)
, frontFace("frontFace", this, gl::GL_CCW)
, blending("blending", this, false)
, renderPass("renderPass", this)
{
    // Invalidate output when input slots have been added or removed
    m_inputAddedConnection = inputAdded.connect([this] (gloperate::AbstractSlot *)
    {
        renderPass.invalidate();
    });

    m_inputRemovedConnection = inputRemoved.connect([this] (gloperate::AbstractSlot *)
    {
        renderPass.invalidate();
    });
}

RenderPassStage::~RenderPassStage()
{
}

void RenderPassStage::onContextInit(AbstractGLContext *)
{
    // Create render pass
    m_renderPass = cppassist::make_unique<gloperate::RenderPass>();
    renderPass.setValue(m_renderPass.get());

    // Create OpenGL state set
    m_beforeState = cppassist::make_unique<globjects::State>(globjects::State::DeferredMode);
    m_renderPass->setStateBefore(m_beforeState.get());
}

void RenderPassStage::onProcess()
{
    // Setup render pass geometry and program
    m_renderPass->setGeometry((*drawable));
    m_renderPass->setProgram((*program));

    // Update OpenGL states
    if (*this->depthTest) m_renderPass->stateBefore()->enable (gl::GL_DEPTH_TEST);
    else                  m_renderPass->stateBefore()->disable(gl::GL_DEPTH_TEST);
    m_renderPass->stateBefore()->depthMask(*this->depthMask ? gl::GL_TRUE : gl::GL_FALSE);
    m_renderPass->stateBefore()->depthFunc(*this->depthFunc);

    if (*this->culling) m_renderPass->stateBefore()->enable (gl::GL_CULL_FACE);
    else                m_renderPass->stateBefore()->disable(gl::GL_CULL_FACE);
    m_renderPass->stateBefore()->cullFace(*this->cullFace);
    m_renderPass->stateBefore()->frontFace(*this->frontFace);

    if (*this->blending) m_renderPass->stateBefore()->enable (gl::GL_BLEND);
    else                 m_renderPass->stateBefore()->disable(gl::GL_BLEND);

    // Update camera uniforms
    if (camera.isValid() && *this->camera)
    {
        gloperate::Camera * camera = *this->camera;

        (*program)->setUniform<glm::vec3>("eye",            camera->eye());
        (*program)->setUniform<glm::mat4>("viewProjection", camera->viewProjection());
        (*program)->setUniform<glm::mat4>("view",           camera->view());
        (*program)->setUniform<glm::mat4>("projection",     camera->projection());
        (*program)->setUniform<glm::mat3>("normalMatrix",   camera->normal());
    }

    // Attach textures
    unsigned int textureIndex = 0;
    for (auto input : inputs<globjects::Texture *>())
    {
        (*program)->setUniform<int>(input->name(), textureIndex);
        m_renderPass->setTexture(textureIndex, **input);

        if ((**input)->target() == gl::GL_TEXTURE_CUBE_MAP)
        {
            m_renderPass->stateBefore()->enable(gl::GL_TEXTURE_CUBE_MAP_SEAMLESS);
        }

        ++textureIndex;
    }

    // Attach shader storage buffers
    unsigned int shaderStorageBufferIndex = 0;
    for (auto bufferInput : inputs<globjects::Buffer *>())
    {
        m_renderPass->setShaderStorageBuffer(shaderStorageBufferIndex, **bufferInput);

        ++shaderStorageBufferIndex;
    }

    // Set color uniforms
    for (auto input : inputs<Color>()) {
        (*program)->setUniform<glm::vec4>(input->name(), (*input)->toVec4());
    }

    for (auto setterPair : uniformSetters)
    {
        setterPair.second();
    }

    // Update outputs
    renderPass.setValue(m_renderPass.get());
}


} // namespace gloperate
