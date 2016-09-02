
#pragma once

#include <unordered_map>
#include <cstdint>
#include <cstddef>

#include <glbinding/gl/types.h>

#include <globjects/base/Referenced.h>
#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate_api.h>


namespace globjects
{

class Buffer;
class Framebuffer;
class Program;
class ProgramPipeline;
class Sampler;
class Texture;
class TransformFeedback;
class State;

} // namespace globjects


namespace gloperate
{

class Drawable;


class GLOPERATE_API RenderPass : public globjects::Referenced
{
public:
    RenderPass();
    virtual ~RenderPass();

    void draw() const;

    globjects::State * state() const;
    void setState(globjects::State * state);

    Drawable * geometry() const;
    void setGeometry(Drawable * geometry);

    globjects::TransformFeedback * recordTransformFeedback() const;
    void setRecordTransformFeedback(globjects::TransformFeedback * transformFeedback);

    gl::GLenum recordTransformFeedbackMode() const;
    void setRecordTransformFeedbackMode(gl::GLenum mode);

    globjects::TransformFeedback * drawTransformFeedback() const;
    void setDrawTransformFeedback(globjects::TransformFeedback * transformFeedback);

    gl::GLenum drawTransformFeedbackMode() const;
    void setDrawTransformFeedbackMode(gl::GLenum mode);

    /**
     * @brief
     *   Accessor for the program used for drawing.
     *
     * @return
     *   The program.
     */
    globjects::Program * program() const;

    /**
     * @brief
     *   Updates the program used for drawing.
     *
     * @param[in] program
     *   The new program to use.
     *
     * @remarks
     *   The passed program is partially owned by this RenderPass.
     *   If both a program and a program pipeline is configured, the program is preferred during rendering.
     */
    void setProgram(globjects::Program * program);

    /**
     * @brief
     *   Accessor for the program pipeline used for drawing.
     *
     * @return
     *   The program pipeline.
     */
    globjects::ProgramPipeline * programPipeline() const;

    /**
     * @brief
     *   Updates the program pipeline used for drawing.
     *
     * @param[in] programPipeline
     *   The new program pipeline to use.
     *
     * @remarks
     *   The passed program pipeline is partially owned by this RenderPass.
     *   If both a program and a program pipeline is configured, the program is preferred during rendering.
     */
    void setProgramPipeline(globjects::ProgramPipeline * programPipeline);

    /**
     * @brief
     *   Accessor for the OpenGL texture object at the given index.
     *
     * @param[in] index
     *   The active texture index.
     *
     * @return
     *   The OpenGL texture object at the given index (nullptr if missing).
     *
     * @remarks
     *   The indices don't need to be continuous.
     */
    globjects::Texture * texture(size_t index) const;

    /**
     * @brief
     *   Accessor for the OpenGL texture object at the given index.
     *
     * @param[in] index
     *   The active texture index as GLenum.
     *
     * @return
     *   The OpenGL texture object at the given index (nullptr if missing).
     *
     * @remarks
     *   The indices don't need to be continuous.
     */
    globjects::Texture * texture(gl::GLenum activeTextureIndex) const;

    /**
     * @brief
     *   Updates a texture that is to be bound active during the draw calls.
     *
     * @param[in] index
     *   The active texture index.
     * @param[in] texture
     *   The texture to be bound.
     *
     * @remarks
     *   To exclude a texture from getting bound active during draw calls, use removeTexture.
     */
    void setTexture(size_t index, globjects::Texture * texture);

    /**
     * @brief
     *   Updates a texture that is to be bound active during the draw calls.
     *
     * @param[in] activeTextureIndex
     *   The active texture index as GLenum.
     * @param[in] texture
     *   The texture to be bound.
     *
     * @remarks
     *   The index is normalized to a size_t so the texture is also available using the size_t interface.
     *   To exclude a texture from getting bound active during draw calls, use removeTexture.
     */
    void setTexture(gl::GLenum activeTextureIndex, globjects::Texture * texture);

    /**
     * @brief
     *   Excludes the texture identified through index from being bound active during the draw calls.
     *
     * @param[in] index
     *   The active texture index.
     *
     * @return
     *   The former texture object associated with this index (may be nullptr).
     */
    globjects::Texture * removeTexture(size_t index);

    /**
     * @brief
     *   Excludes the texture identified through index from being bound active during the draw calls.
     *
     * @param[in] index
     *   The active texture index as GLenum.
     *
     * @return
     *   The former texture object associated with this index (may be nullptr).
     *
     * @remarks
     *   The index is converted to a size_t so a texture configured through the size_t interface is also affected.
     */
    globjects::Texture * removeTexture(gl::GLenum activeTextureIndex);

    globjects::Sampler * sampler(size_t index) const;
    void setSampler(size_t index, globjects::Sampler * texture);
    globjects::Sampler * removeSampler(size_t index);

    globjects::Buffer * uniformBuffer(size_t index) const;
    void setUniformBuffers(size_t index, globjects::Buffer * texture);
    globjects::Buffer * removeUniformBuffer(size_t index);

    globjects::Buffer * atomicCounterBuffer(size_t index) const;
    void setAtomicCounterBuffer(size_t index, globjects::Buffer * texture);
    globjects::Buffer * removeAtomicCounterBuffer(size_t index);

    globjects::Buffer * shaderStorageBuffer(size_t index) const;
    void setShaderStorageBuffer(size_t index, globjects::Buffer * texture);
    globjects::Buffer * removeShaderStorageBuffer(size_t index);

    globjects::Buffer * transformFeedbackBuffer(size_t index) const;
    void setTransformFeedbackBuffer(size_t index, globjects::Buffer * texture);
    globjects::Buffer * removeTransformFeedbackBuffer(size_t index);

protected:
    globjects::ref_ptr<globjects::State> m_state;
    globjects::ref_ptr<Drawable> m_geometry;
    globjects::ref_ptr<globjects::Program> m_program;
    globjects::ref_ptr<globjects::ProgramPipeline> m_programPipeline;
    globjects::ref_ptr<globjects::TransformFeedback> m_recordTransformFeedback;
    gl::GLenum m_recordTransformFeedbackMode;
    globjects::ref_ptr<globjects::TransformFeedback> m_drawTransformFeedback;
    gl::GLenum m_drawTransformFeedbackMode;

    std::unordered_map<size_t, globjects::ref_ptr<globjects::Texture>> m_textures; /// The collection of all textures associated with this render pass. The key is used as the active texture binding.
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Sampler>> m_samplers; /// The collection of all samplers associated with this render pass. The key is used as the sampler binding index.
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Buffer>> m_uniformBuffers; /// The collection of all uniform buffers associated with this render pass. The key is used as the uniform buffer binding index.
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Buffer>> m_atomicCounterBuffers; /// The collection of all atomic counter buffers associated with this render pass. The key is used as the atomic counter buffer binding index.
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Buffer>> m_shaderStorageBuffers; /// The collection of all shader storage buffers associated with this render pass. The key is used as the shader storage buffer binding index.
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Buffer>> m_transformFeedbackBuffers; /// The collection of all transform feedback buffers associated with this render pass. The key is used as the transform feedback buffer binding index.

protected:
    void bindResources() const;

};


} // namespace gloperate
