
#pragma once

#include <unordered_map>
#include <cstdint>

#include <globjects/base/Referenced.h>
#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate_api.h>


namespace globjects
{

class Framebuffer;
class Program;
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

    // Program Pipeline
    // Uniform Buffer
    // Uniform Group
    // Atomic Counter Buffer
    // Shader Storage Buffer
    // Global State / Capabilities
    // Framebuffer Attachments (Textures / Renderbuffers / ?)
    // Textures / Images / Samplers / Texture Views
    // Bindless Textures?
    // Queries?
    // Command Lists?

protected:
    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
    globjects::ref_ptr<MyDrawable> m_geometry;
    globjects::ref_ptr<globjects::Program> m_program;

    std::unordered_map<size_t, globjects::ref_ptr<globjects::Texture>> m_textures; /// The collection of all textures associated with this render pass. The key is used as the active texture binding.

protected:
    void bindTextures() const;

};

// TODO: Compute pass? Superclass of RenderPass? common superclass named GPUPass?


} // namespace gloperate
