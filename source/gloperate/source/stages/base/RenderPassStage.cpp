
#include <gloperate/stages/base/RenderPassStage.h>

#include <glbinding/gl/enum.h>

#include <globjects/Program.h>
#include <globjects/State.h>
#include <globjects/Texture.h>
#include <globjects/TextureHandle.h>

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

    // Update uniforms from dynamic inputs
    unsigned int textureIndex = 0;
    unsigned int shaderStorageBufferIndex = 0;

    for (auto input : inputs())
    {
        // Only conside dynamic inputs here
        if (!input->isDynamic())
            continue;

        // Texture
        if (input->type() == typeid(globjects::Texture *))
        {
            // Get texture input
            auto textureInput = static_cast<Input<globjects::Texture *> *>(input);

            // Attach texture
            (*program)->setUniform<int>(input->name(), textureIndex);
            m_renderPass->setTexture(textureIndex, **textureInput);

            if ((**textureInput)->target() == gl::GL_TEXTURE_CUBE_MAP)
            {
                m_renderPass->stateBefore()->enable(gl::GL_TEXTURE_CUBE_MAP_SEAMLESS);
            }

            ++textureIndex;
        }

        // Shader storage buffer
        else if (input->type() == typeid(globjects::Buffer *))
        {
            // Get buffer input
            auto bufferInput = static_cast<Input<globjects::Buffer *> *>(input);

            // Attach shader storage buffer
            m_renderPass->setShaderStorageBuffer(shaderStorageBufferIndex, **bufferInput);
            ++shaderStorageBufferIndex;
        }

        // Color
        else if (input->type() == typeid(Color))
        {
            // Get color input
            auto colorInput = static_cast<Input<Color> *>(input);

            // Set color uniform
            (*program)->setUniform<glm::vec4>(input->name(), (*colorInput)->toVec4());
        }

        // Basic uniform
        else
        {
            setUniformValue(*program, input);
        }
    }

    // Update outputs
    renderPass.setValue(m_renderPass.get());
}

void RenderPassStage::setUniformValue(globjects::Program * program, AbstractSlot * input)
{
    if (input->type() == typeid(float)) {
        program->setUniform<float>(input->name(), static_cast<Input<float> *>(input)->value());
    } else if (input->type() == typeid(int)) {
        program->setUniform<int>(input->name(), static_cast<Input<int> *>(input)->value());
    } else if (input->type() == typeid(unsigned int)) {
        program->setUniform<unsigned int>(input->name(), static_cast<Input<unsigned int> *>(input)->value());
    } else if (input->type() == typeid(bool)) {
        program->setUniform<bool>(input->name(), static_cast<Input<bool> *>(input)->value());
    } else if (input->type() == typeid(glm::vec2)) {
        program->setUniform<glm::vec2>(input->name(), static_cast<Input<glm::vec2> *>(input)->value());
    } else if (input->type() == typeid(glm::vec3)) {
        program->setUniform<glm::vec3>(input->name(), static_cast<Input<glm::vec3> *>(input)->value());
    } else if (input->type() == typeid(glm::vec4)) {
        program->setUniform<glm::vec4>(input->name(), static_cast<Input<glm::vec4> *>(input)->value());
    } else if (input->type() == typeid(glm::ivec2)) {
        program->setUniform<glm::ivec2>(input->name(), static_cast<Input<glm::ivec2> *>(input)->value());
    } else if (input->type() == typeid(glm::ivec3)) {
        program->setUniform<glm::ivec3>(input->name(), static_cast<Input<glm::ivec3> *>(input)->value());
    } else if (input->type() == typeid(glm::ivec4)) {
        program->setUniform<glm::ivec4>(input->name(), static_cast<Input<glm::ivec4> *>(input)->value());
    } else if (input->type() == typeid(glm::uvec2)) {
        program->setUniform<glm::uvec2>(input->name(), static_cast<Input<glm::uvec2> *>(input)->value());
    } else if (input->type() == typeid(glm::uvec3)) {
        program->setUniform<glm::uvec3>(input->name(), static_cast<Input<glm::uvec3> *>(input)->value());
    } else if (input->type() == typeid(glm::uvec4)) {
        program->setUniform<glm::uvec4>(input->name(), static_cast<Input<glm::uvec4> *>(input)->value());
    } else if (input->type() == typeid(glm::mat2)) {
        program->setUniform<glm::mat2>(input->name(), static_cast<Input<glm::mat2> *>(input)->value());
    } else if (input->type() == typeid(glm::mat3)) {
        program->setUniform<glm::mat3>(input->name(), static_cast<Input<glm::mat3> *>(input)->value());
    } else if (input->type() == typeid(glm::mat4)) {
        program->setUniform<glm::mat4>(input->name(), static_cast<Input<glm::mat4> *>(input)->value());
    } else if (input->type() == typeid(glm::mat2x3)) {
        program->setUniform<glm::mat2x3>(input->name(), static_cast<Input<glm::mat2x3> *>(input)->value());
    } else if (input->type() == typeid(glm::mat3x2)) {
        program->setUniform<glm::mat3x2>(input->name(), static_cast<Input<glm::mat3x2> *>(input)->value());
    } else if (input->type() == typeid(glm::mat2x4)) {
        program->setUniform<glm::mat2x4>(input->name(), static_cast<Input<glm::mat2x4> *>(input)->value());
    } else if (input->type() == typeid(glm::mat4x2)) {
        program->setUniform<glm::mat4x2>(input->name(), static_cast<Input<glm::mat4x2> *>(input)->value());
    } else if (input->type() == typeid(glm::mat3x4)) {
        program->setUniform<glm::mat3x4>(input->name(), static_cast<Input<glm::mat3x4> *>(input)->value());
    } else if (input->type() == typeid(glm::mat4x3)) {
        program->setUniform<glm::mat4x3>(input->name(), static_cast<Input<glm::mat4x3> *>(input)->value());
    } else if (input->type() == typeid(gl::GLuint64)) {
        program->setUniform<gl::GLuint64>(input->name(), static_cast<Input<gl::GLuint64> *>(input)->value());
    } else if (input->type() == typeid(globjects::TextureHandle)) {
        program->setUniform<globjects::TextureHandle>(input->name(), static_cast<Input<globjects::TextureHandle> *>(input)->value());
    } else if (input->type() == typeid(std::vector<float>)) {
        program->setUniform<std::vector<float>>(input->name(), static_cast<Input<std::vector<float>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<int>)) {
        program->setUniform<std::vector<int>>(input->name(), static_cast<Input<std::vector<int>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<unsigned int>)) {
        program->setUniform<std::vector<unsigned int>>(input->name(), static_cast<Input<std::vector<unsigned int>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<bool>)) {
        program->setUniform<std::vector<bool>>(input->name(), static_cast<Input<std::vector<bool>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::vec2>)) {
        program->setUniform<std::vector<glm::vec2>>(input->name(), static_cast<Input<std::vector<glm::vec2>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::vec3>)) {
        program->setUniform<std::vector<glm::vec3>>(input->name(), static_cast<Input<std::vector<glm::vec3>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::vec4>)) {
        program->setUniform<std::vector<glm::vec4>>(input->name(), static_cast<Input<std::vector<glm::vec4>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::ivec2>)) {
        program->setUniform<std::vector<glm::ivec2>>(input->name(), static_cast<Input<std::vector<glm::ivec2>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::ivec3>)) {
        program->setUniform<std::vector<glm::ivec3>>(input->name(), static_cast<Input<std::vector<glm::ivec3>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::ivec4>)) {
        program->setUniform<std::vector<glm::ivec4>>(input->name(), static_cast<Input<std::vector<glm::ivec4>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::uvec2>)) {
        program->setUniform<std::vector<glm::uvec2>>(input->name(), static_cast<Input<std::vector<glm::uvec2>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::uvec3>)) {
        program->setUniform<std::vector<glm::uvec3>>(input->name(), static_cast<Input<std::vector<glm::uvec3>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::uvec4>)) {
        program->setUniform<std::vector<glm::uvec4>>(input->name(), static_cast<Input<std::vector<glm::uvec4>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::mat2>)) {
        program->setUniform<std::vector<glm::mat2>>(input->name(), static_cast<Input<std::vector<glm::mat2>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::mat3>)) {
        program->setUniform<std::vector<glm::mat3>>(input->name(), static_cast<Input<std::vector<glm::mat3>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::mat4>)) {
        program->setUniform<std::vector<glm::mat4>>(input->name(), static_cast<Input<std::vector<glm::mat4>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::mat2x3>)) {
        program->setUniform<std::vector<glm::mat2x3>>(input->name(), static_cast<Input<std::vector<glm::mat2x3>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::mat3x2>)) {
        program->setUniform<std::vector<glm::mat3x2>>(input->name(), static_cast<Input<std::vector<glm::mat3x2>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::mat2x4>)) {
        program->setUniform<std::vector<glm::mat2x4>>(input->name(), static_cast<Input<std::vector<glm::mat2x4>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::mat4x2>)) {
        program->setUniform<std::vector<glm::mat4x2>>(input->name(), static_cast<Input<std::vector<glm::mat4x2>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::mat3x4>)) {
        program->setUniform<std::vector<glm::mat3x4>>(input->name(), static_cast<Input<std::vector<glm::mat3x4>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<glm::mat4x3>)) {
        program->setUniform<std::vector<glm::mat4x3>>(input->name(), static_cast<Input<std::vector<glm::mat4x3>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<gl::GLuint64>)) {
        program->setUniform<std::vector<gl::GLuint64>>(input->name(), static_cast<Input<std::vector<gl::GLuint64>> *>(input)->value());
    } else if (input->type() == typeid(std::vector<globjects::TextureHandle>)) {
        program->setUniform<std::vector<globjects::TextureHandle>>(input->name(), static_cast<Input<std::vector<globjects::TextureHandle>> *>(input)->value());
    }
}


} // namespace gloperate
