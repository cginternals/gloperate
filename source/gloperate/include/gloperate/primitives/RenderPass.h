
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

} // namespace globjects


namespace gloperate
{

class MyDrawable;


class GLOPERATE_API RenderPass : public globjects::Referenced
{
public:
    RenderPass();

    /**
     * @return the OpenGL texture object at the given index
     * @param index the active texture index
     *
     * The indices don't need to be continuous.
     * If an OpenGL texture at the given index doesn't exist a new one will be created
     */
    globjects::Texture * texture(size_t index);

    /**
     * @return the OpenGL texture object at the given index
     * @param index the active texture index
     *
     * The indices don't need to be continuous.
     * If an OpenGL texture at the given index doesn't exist an exception is thrown
     */
    globjects::Texture * texture(size_t index) const;

    /**
     * @return the OpenGL texture object at the given active texture index
     * @param activeTextureIndex the active texture index as GLenum
     *
     * Note: The index is normalized to a size_t so the texture is also available using the size_t interface.
     *
     * The indices don't need to be continuous.
     * If an OpenGL texture at the given index doesn't exist a new one will be created
     */
    globjects::Texture * texture(gl::GLenum activeTextureIndex);

    /**
     * @return the OpenGL texture object at the given active texture index
     * @param activeTextureIndex the active texture index as GLenum
     *
     * Note: The index is normalized to a size_t so the texture is also available using the size_t interface.
     *
     * The indices don't need to be continuous.
     * If an OpenGL texture at the given index doesn't exist an exception is thrown
     */
    globjects::Texture * texture(gl::GLenum activeTextureIndex) const;

    /**
     * @brief updates a texture that is to be bound active during the draw calls
     * @param index the active texture index
     * @param texture the texture to be bound
     *
     * Hint: To exclude a texture from getting bound active during draw calls, use removeTexture.
     */
    void setTexture(size_t index, globjects::Texture * texture);

    /**
     * @brief updates a texture that is to be bound active during the draw calls
     * @param activeTextureIndex the active texture index as GLenum
     * @param texture the texture to be bound
     *
     * Note: The index is normalized to a size_t so the texture is also available using the size_t interface.
     * Hint: To exclude a texture from getting bound active during draw calls, use removeTexture.
     */
    void setTexture(gl::GLenum activeTextureIndex, globjects::Texture * texture);

    /**
     * @brief excludes the texture identified through index from being bound active during the draw calls.
     * @param index the active texture index
     * @return the former texture object associated with this index
     */
    globjects::Texture * removeTexture(size_t index);

    /**
     * @brief excludes the texture identified through index from being bound active during the draw calls.
     * @param activeTextureIndex the active texture index as GLenum
     * @return the former texture object associated with this index
     *
     * Note: The index is normalized to a size_t so a texture configured through the size_t interface is also affected.
     */
    globjects::Texture * removeTexture(gl::GLenum activeTextureIndex);

    globjects::Program * program() const;
    void setProgram(globjects::Program * program) const;

    globjects::ProgramPipeline * programPipeline() const;
    void setProgramPipeline(globjects::ProgramPipeline * programPipeline) const;

    globjects::Texture * sampler(size_t index);
    globjects::Texture * sampler(size_t index) const;
    void setSampler(size_t index, globjects::Texture * texture);
    globjects::Texture * removeSampler(size_t index);

    globjects::Texture * uniformBuffer(size_t index);
    globjects::Texture * uniformBuffer(size_t index) const;
    void setUniformBuffers(size_t index, globjects::Texture * texture);
    globjects::Texture * removeUniformBuffer(size_t index);

    globjects::Texture * atomicCounterBuffer(size_t index);
    globjects::Texture * atomicCounterBuffer(size_t index) const;
    void setAtomicCounterBuffer(size_t index, globjects::Texture * texture);
    globjects::Texture * removeAtomicCounterBuffer(size_t index);

    globjects::Texture * shaderStorageBuffer(size_t index);
    globjects::Texture * shaderStorageBuffer(size_t index) const;
    void setShaderStorageBuffer(size_t index, globjects::Texture * texture);
    globjects::Texture * removeShaderStorageBuffer(size_t index);

protected:
    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
    globjects::ref_ptr<MyDrawable> m_geometry;
    globjects::ref_ptr<globjects::Program> m_program;
    globjects::ref_ptr<globjects::ProgramPipeline> m_programPipeline;

    std::unordered_map<size_t, globjects::ref_ptr<globjects::Texture>> m_textures; /// The collection of all textures associated with this render pass. The key is used as the active texture binding.
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Sampler>> m_samplers;
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Buffer>> m_uniformBuffers;
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Buffer>> m_atomicCounterBuffers;
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Buffer>> m_shaderStorageBuffers;

protected:
    void bindTextures() const;

};


} // namespace gloperate
