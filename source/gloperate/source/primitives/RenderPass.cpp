
#include <gloperate/primitives/RenderPass.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <globjects/Texture.h>
#include <globjects/Sampler.h>
#include <globjects/Buffer.h>
#include <globjects/State.h>
#include <globjects/VertexArray.h>
#include <globjects/Program.h>
#include <globjects/ProgramPipeline.h>
#include <globjects/TransformFeedback.h>

#include <gloperate/primitives/Drawable.h>


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

    if (m_recordTransformFeedback)
    {
        m_recordTransformFeedback->bind();
        m_recordTransformFeedback->begin(m_recordTransformFeedbackMode);

        gl::glEnable(gl::GL_RASTERIZER_DISCARD);
    }
    else
    {
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
}

globjects::State * RenderPass::state() const
{
    return m_state;
}

void RenderPass::setState(globjects::State * state)
{
    m_state = state;
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
    m_textures.emplace(index, texture);
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

void RenderPass::setSampler(size_t index, globjects::Sampler * texture)
{
    m_samplers.emplace(index, texture);
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

void RenderPass::setUniformBuffers(size_t index, globjects::Buffer * texture)
{
    m_uniformBuffers.emplace(index, texture);
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

void RenderPass::setAtomicCounterBuffer(size_t index, globjects::Buffer * texture)
{
    m_atomicCounterBuffers.emplace(index, texture);
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

void RenderPass::setShaderStorageBuffer(size_t index, globjects::Buffer * texture)
{
    m_shaderStorageBuffers.emplace(index, texture);
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

void RenderPass::setTransformFeedbackBuffer(size_t index, globjects::Buffer * texture)
{
    m_transformFeedbackBuffers.emplace(index, texture);
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

    if (m_state)
    {
        m_state->apply();
    }
}


} // namespace gloperate
