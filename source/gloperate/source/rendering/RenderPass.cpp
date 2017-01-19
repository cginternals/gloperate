
#include <gloperate/rendering/RenderPass.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <globjects/Texture.h>
#include <globjects/Sampler.h>
#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/Program.h>
#include <globjects/ProgramPipeline.h>
#include <globjects/TransformFeedback.h>
#include <globjects/State.h>

#include <gloperate/rendering/Drawable.h>


namespace gloperate
{


RenderPass::RenderPass()
: m_recordTransformFeedbackMode(gl::GL_POINTS)
, m_drawTransformFeedbackMode(gl::GL_POINTS)
{
}

RenderPass::~RenderPass()
{
}

void RenderPass::draw() const
{
    bindResources();
    
    if (m_stateBefore)
    {
        m_stateBefore->apply();
    }

    if (m_recordTransformFeedback)
    {
        m_recordTransformFeedback->bind();
        m_recordTransformFeedback->begin(m_recordTransformFeedbackMode);

        gl::glEnable(gl::GL_RASTERIZER_DISCARD);
    }

    if (m_program)
    {
        m_program->use();
    }
    else if (m_programPipeline)
    {
        m_programPipeline->use();
    }
    else
    {
        globjects::Program::release();
        globjects::ProgramPipeline::release();
    }

    if (m_drawTransformFeedback)
    {
        m_drawTransformFeedback->draw(m_drawTransformFeedbackMode);
    }
    else
    {
        m_geometry->draw();
    }

    if (m_recordTransformFeedback)
    {
        m_recordTransformFeedback->end();

        gl::glDisable(gl::GL_RASTERIZER_DISCARD);
    }
    
    if (m_stateAfter)
    {
        m_stateAfter->apply();
    }
}

globjects::State * RenderPass::stateBefore() const
{
    return m_stateBefore;
}

void RenderPass::setStateBefore(globjects::State * state)
{
    m_stateBefore = state;
}

globjects::State * RenderPass::stateAfter() const
{
    return m_stateAfter;
}

void RenderPass::setStateAfter(globjects::State * state)
{
    m_stateAfter = state;
}

Drawable * RenderPass::geometry() const
{
    return m_geometry;
}

void RenderPass::setGeometry(Drawable * geometry)
{
    m_geometry = geometry;
}

globjects::TransformFeedback * RenderPass::recordTransformFeedback() const
{
    return m_recordTransformFeedback;
}

void RenderPass::setRecordTransformFeedback(globjects::TransformFeedback * transformFeedback)
{
    m_recordTransformFeedback = transformFeedback;
}

gl::GLenum RenderPass::recordTransformFeedbackMode() const
{
    return m_recordTransformFeedbackMode;
}

void RenderPass::setRecordTransformFeedbackMode(gl::GLenum mode)
{
    m_recordTransformFeedbackMode = mode;
}

globjects::TransformFeedback * RenderPass::drawTransformFeedback() const
{
    return m_drawTransformFeedback;
}

void RenderPass::setDrawTransformFeedback(globjects::TransformFeedback * transformFeedback)
{
    m_drawTransformFeedback = transformFeedback;
}

gl::GLenum RenderPass::drawTransformFeedbackMode() const
{
    return m_drawTransformFeedbackMode;
}

void RenderPass::setDrawTransformFeedbackMode(gl::GLenum mode)
{
    m_drawTransformFeedbackMode = mode;
}

globjects::Program * RenderPass::program() const
{
    return m_program;
}

void RenderPass::setProgram(globjects::Program * program)
{
    m_program = program;
}

globjects::ProgramPipeline * RenderPass::programPipeline() const
{
    return m_programPipeline;
}

void RenderPass::setProgramPipeline(globjects::ProgramPipeline * programPipeline)
{
    m_programPipeline = programPipeline;
}

globjects::Texture * RenderPass::texture(size_t index) const
{
    const auto it = m_textures.find(index);

    if (it == m_textures.end())
    {
        return nullptr;
    }

    return it->second;
}

globjects::Texture * RenderPass::texture(gl::GLenum activeTextureIndex) const
{
    return texture(static_cast<size_t>(activeTextureIndex) - static_cast<size_t>(gl::GL_TEXTURE0));
}

void RenderPass::setTexture(size_t index, globjects::Texture * texture)
{
    const auto it = m_textures.find(index);

    if (it == m_textures.end())
    {
        m_textures.emplace(index, texture);
    }
    else
    {
        it->second = texture;
    }
}

void RenderPass::setTexture(gl::GLenum activeTextureIndex, globjects::Texture * texture)
{
    return setTexture(static_cast<size_t>(activeTextureIndex) - static_cast<size_t>(gl::GL_TEXTURE0), texture);
}

globjects::Texture * RenderPass::removeTexture(size_t index)
{
    const auto it = m_textures.find(index);

    if (it == m_textures.end())
    {
        return nullptr;
    }

    const auto former = it->second.get();

    m_textures.erase(it);

    return former;
}

globjects::Texture * RenderPass::removeTexture(gl::GLenum activeTextureIndex)
{
    return removeTexture(static_cast<size_t>(activeTextureIndex) - static_cast<size_t>(gl::GL_TEXTURE0));
}

globjects::Sampler * RenderPass::sampler(size_t index) const
{
    const auto it = m_samplers.find(index);

    if (it == m_samplers.end())
    {
        return nullptr;
    }

    return it->second.get();
}

void RenderPass::setSampler(size_t index, globjects::Sampler * sampler)
{
    const auto it = m_samplers.find(index);

    if (it == m_samplers.end())
    {
        m_samplers.emplace(index, sampler);
    }
    else
    {
        it->second = sampler;
    }
}

globjects::Sampler * RenderPass::removeSampler(size_t index)
{
    const auto it = m_samplers.find(index);

    if (it == m_samplers.end())
    {
        return nullptr;
    }

    const auto former = it->second.get();

    m_samplers.erase(it);

    return former;
}

globjects::Buffer * RenderPass::uniformBuffer(size_t index) const
{
    const auto it = m_uniformBuffers.find(index);

    if (it == m_uniformBuffers.end())
    {
        return nullptr;
    }

    return it->second;
}

void RenderPass::setUniformBuffer(size_t index, globjects::Buffer * buffer)
{
    const auto it = m_uniformBuffers.find(index);

    if (it == m_uniformBuffers.end())
    {
        m_uniformBuffers.emplace(index, buffer);
    }
    else
    {
        it->second = buffer;
    }
}

globjects::Buffer * RenderPass::removeUniformBuffer(size_t index)
{
    const auto it = m_uniformBuffers.find(index);

    if (it == m_uniformBuffers.end())
    {
        return nullptr;
    }

    const auto former = it->second.get();

    m_uniformBuffers.erase(it);

    return former;
}

globjects::Buffer * RenderPass::atomicCounterBuffer(size_t index) const
{
    const auto it = m_atomicCounterBuffers.find(index);

    if (it == m_atomicCounterBuffers.end())
    {
        return nullptr;
    }

    return it->second;
}

void RenderPass::setAtomicCounterBuffer(size_t index, globjects::Buffer * buffer)
{
    const auto it = m_atomicCounterBuffers.find(index);

    if (it == m_atomicCounterBuffers.end())
    {
        m_atomicCounterBuffers.emplace(index, buffer);
    }
    else
    {
        it->second = buffer;
    }
}

globjects::Buffer * RenderPass::removeAtomicCounterBuffer(size_t index)
{
    const auto it = m_atomicCounterBuffers.find(index);

    if (it == m_atomicCounterBuffers.end())
    {
        return nullptr;
    }

    const auto former = it->second.get();

    m_atomicCounterBuffers.erase(it);

    return former;
}

globjects::Buffer * RenderPass::shaderStorageBuffer(size_t index) const
{
    const auto it = m_shaderStorageBuffers.find(index);

    if (it == m_shaderStorageBuffers.end())
    {
        return nullptr;
    }

    return it->second;
}

void RenderPass::setShaderStorageBuffer(size_t index, globjects::Buffer * buffer)
{
    const auto it = m_shaderStorageBuffers.find(index);

    if (it == m_shaderStorageBuffers.end())
    {
        m_shaderStorageBuffers.emplace(index, buffer);
    }
    else
    {
        it->second = buffer;
    }
}

globjects::Buffer * RenderPass::removeShaderStorageBuffer(size_t index)
{
    const auto it = m_shaderStorageBuffers.find(index);

    if (it == m_shaderStorageBuffers.end())
    {
        return nullptr;
    }

    const auto former = it->second.get();

    m_shaderStorageBuffers.erase(it);

    return former;
}

globjects::Buffer * RenderPass::transformFeedbackBuffer(size_t index) const
{
    const auto it = m_transformFeedbackBuffers.find(index);

    if (it == m_transformFeedbackBuffers.end())
    {
        return nullptr;
    }

    return it->second;
}

void RenderPass::setTransformFeedbackBuffer(size_t index, globjects::Buffer * buffer)
{
    const auto it = m_transformFeedbackBuffers.find(index);

    if (it == m_transformFeedbackBuffers.end())
    {
        m_transformFeedbackBuffers.emplace(index, buffer);
    }
    else
    {
        it->second = buffer;
    }
}

globjects::Buffer * RenderPass::removeTransformFeedbackBuffer(size_t index)
{
    const auto it = m_transformFeedbackBuffers.find(index);

    if (it == m_transformFeedbackBuffers.end())
    {
        return nullptr;
    }

    const auto former = it->second.get();

    m_transformFeedbackBuffers.erase(it);

    return former;
}

void RenderPass::bindResources() const
{
    for (const auto & pair : m_textures)
    {
        pair.second->bindActive(pair.first);
    }

    for (const auto & pair : m_samplers)
    {
        pair.second->bind(pair.first);
    }

    for (const auto & pair : m_uniformBuffers)
    {
        pair.second->bindBase(gl::GL_UNIFORM_BUFFER, pair.first);
    }

    for (const auto & pair : m_atomicCounterBuffers)
    {
        pair.second->bindBase(gl::GL_ATOMIC_COUNTER_BUFFER, pair.first);
    }

    for (const auto & pair : m_shaderStorageBuffers)
    {
        pair.second->bindBase(gl::GL_SHADER_STORAGE_BUFFER, pair.first);
    }

    for (const auto & pair : m_transformFeedbackBuffers)
    {
        pair.second->bindBase(gl::GL_TRANSFORM_FEEDBACK_BUFFER, pair.first);
    }
}


} // namespace gloperate
