
#pragma once

#include <vector>
#include <unordered_map>
#include <array>
#include <type_traits>
#include <cstdint>

#include <glbinding/gl/types.h>

#include <globjects/Buffer.h>
#include <globjects/Texture.h>
#include <globjects/VertexArray.h>
#include <globjects/base/Referenced.h>
#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate_api.h>

#include <gloperate/primitives/AbstractDrawable.h>

namespace gloperate
{

/**
 * @brief The DrawMode enum
 *   Used to specify the draw mode used in the MyDrawable class.
 */
enum class DrawMode : unsigned int
{
    Arrays, /// dispatches to glDrawArrays
    ElementsIndices, /// dispatches to glDrawElements using a CPU index buffer
    ElementsIndexBuffer /// dispatches to glDrawElements using a GPU index buffer
};

/**
 * @brief The MyDrawable class wraps the setup and drawing routine for Vertex Arrays, Vertex Attribute Bindings and Buffers.
 *   Supported drawing types:
 *    * glDrawArrays
 *    * glDrawElements using CPU index buffer
 *    * glDrawElements using GPU index buffer
 *   Supported buffer arrangements:
 *    * Separate buffer per vertex attribute
 *    * Interleaved buffer for all vertex attributes
 *    * Arbitrary distribution of vertex attributes to buffers (mixed separate and interleaved buffers)
 *
 *   Note: most configurable parameters (as DrawMode, primitive mode, draw count, index buffer source, ...) can be temporarily overwritten for each draw call.
 */
class MyDrawable : public globjects::Referenced, gloperate::AbstractDrawable
{
public:
    /**
     * @brief Constructor of MyDrawable
     *   Creates OpenGL objects, thus, a current context is required
     */
    MyDrawable();

    /**
      * @brief Destructor
      */
    ~MyDrawable();

    /**
     * @brief A convenience accessor for the internal VertexArray instance.
     * @return the VertexArray instance
     */
    globjects::VertexArray * vao() const;

    /**
     * @return the current configured DrawMode
     */
    DrawMode drawMode() const;

    /**
     * @brief updates the configured DrawMode to distinguish the main intended OpenGL draw call
     * @param drawMode the new draw mode
     */
    void setDrawMode(DrawMode drawMode);

    /**
     * @brief the main draw entry point, used by the superclass AbstractDrawable
     *   Internally, this method dispatches to the other draw* methods based on the currentl drawMode.
     */
    virtual void draw() const override;

    /**
     * @brief A second entry point where a different drawMode than the currently configured can be used.
     * @param drawMode the drawMode to be used for dispatching this specific draw call
     */
    void draw(DrawMode drawMode) const;

    /**
     * @brief the draw entry point for per vertex based drawing.
     *   It triggers a glDrawArrays draw call with the currently configured mode using the full vertex buffers.
     */
    void drawArrays() const;

    /**
     * @brief Another draw entry point for per vertex based drawing where a different primitive mode than the currently configured can be used.
     * @param mode the primitive mode to be used for this specific draw call
     */
    void drawArrays(gl::GLenum mode) const;

    /**
     * @brief Another draw entry point for per vertex based drawing where a different vertex buffer range than the currently configured can be used.
     * @param first the index of the first vertex to be used for this specific draw call
     * @param count the number of vertices to be used for this specific draw call
     */
    void drawArrays(gl::GLint first, gl::GLsizei count) const;

    /**
     * @brief Another draw entry point for per vertex based drawing where a different primitive mode and vertex buffer range than the currently configured can be used.
     * @param mode the primitive mode to be used for this specific draw call
     * @param first the index of the first vertex to be used for this specific draw call
     * @param count the number of vertices to be used for this specific draw call
     */
    void drawArrays(gl::GLenum mode, gl::GLint first, gl::GLsizei count) const;

    /**
     * @brief the draw entry point for index based drawing.
     *   It triggers a glDrawElements draw call with the currently configured mode and index buffer (either CPU or GPU) using the full index range.
     */
    void drawElements() const;

    /**
     * @brief another draw entry point for index based drawing where a different primitive mode than the currently configured can be used.
     * @param mode the primitive mode to be used for this specific draw call
     */
    void drawElements(gl::GLenum mode) const;

    /**
     * @brief another draw entry point for index based drawing where a different primitive mode than the currently configured and a specific CPU index buffer can be used.
     * @param mode the primitive mode to be used for this specific draw call
     * @param count the number of indices to use for this specific draw call
     * @param type the data type of the indices to use for this specific draw call
     * @param indices the pointer to the index buffer, containing adjacent values of the type passed in as type parameter
     */
    void drawElements(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices) const;

    /**
     * @brief another draw entry point for index based drawing where a different primitive mode than the currently configured and a specific GPU index buffer can be used.
     * @param mode the primitive mode to be used for this specific draw call
     * @param count the number of indices to use for this specific draw call
     * @param type the data type of the indices to use for this specific draw call
     * @param indices the OpenGL buffer containing the indices, which are adjacent values of the type passed in as type parameter
     */
    void drawElements(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, globjects::Buffer * indices) const;

    /**
     * @return the configured vertex count of the geometry (usually that is the count parameter for the associated draw calls)
     */
    gl::GLsizei size() const;

    /**
     * @brief updates the configured vertex count of the geometry
     * @param size the new size
     *
     * Note: this size must not be equal to the maximum length of one or all associated buffers but is advised not to exceed the ranges of any buffer.
     */
    void setSize(gl::GLsizei size);

    /**
     * @return the currently configured draw call primitive mode
     */
    gl::GLenum mode() const;

    /**
     * @brief updates the configured primitive mode used in the draw calls
     * @param mode the new primitive mode
     */
    void setMode(gl::GLenum mode);

    /**
     * @brief convenience method to initialize or update a vertex buffer
     * @param index the index of the vertex buffer
     * @param data the new data
     *
     * The indices don't need to be continuous.
     */
    template <typename VectorType>
    void setData(size_t index, const std::vector<VectorType> & data);

    /**
     * @brief convenience method to initialize or update a vertex buffer
     * @param index the index of the vertex buffer
     * @param data the new data
     */
    template <typename ArrayType, size_t ArraySize>
    void setData(size_t index, const std::array<ArrayType, ArraySize> & data);

    /**
     * @return the OpenGL buffer at the given index
     * @param index the index of the OpenGL buffer
     *
     * The indices don't need to be continuous.
     * If an OpenGL buffer at the given index doesn't exist a new one will be created
     */
    globjects::Buffer * buffer(size_t index);

    /**
     * @return the OpenGL buffer at the given index
     * @param index the index of the OpenGL buffer
     *
     * The indices don't need to be continuous.
     * If an OpenGL buffer at the given index doesn't exist an exception is thrown
     */
    globjects::Buffer * buffer(size_t index) const;

    /**
     * @brief associated an OpenGL buffer with an index.
     * @param index the index of the OpenGL buffer
     * @param buffer the buffer to associate
     *
     * Note: The indices don't need to be continuous and one buffer can be assigned to more than one index.
     * Hint: A buffer doesn't need to be associated with an index to be usable by this class. However if it is associated, the buffer object is partially owned by this object.
     */
    void setBuffer(size_t index, globjects::Buffer * buffer);

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

    /**
     * @return the index buffer.
     *
     * The return value may be a null pointer
     */
    globjects::Buffer * indexBuffer() const;

    /**
     * @brief updates the configured index buffer
     * @param buffer the new OpenGL index buffer
     */
    void setIndexBuffer(globjects::Buffer * buffer);

    /**
     * @brief updates the configured index buffer and its type
     * @param buffer the new OpenGL index buffer
     * @param bufferType the type of the indices
     */
    void setIndexBuffer(globjects::Buffer * buffer, gl::GLenum bufferType);

    /**
     * @return the currently configured GPU index data type
     */
    gl::GLenum indexBufferType() const;

    /**
     * @brief updates the configured index data type
     * @param bufferType the new data type
     */
    void setIndexBufferType(gl::GLenum bufferType);

    /**
     * @return the currently configured CPU index buffer
     */
    const std::vector<std::uint32_t> & indices() const;

    /**
     * @brief updated the configured CPU index buffer
     * @param indices the vector of indices (needs to be of type GL_UNSIGNED_INT)
     */
    void setIndices(const std::vector<std::uint32_t> & indices);

    /**
     * @return the vertex attribute binding at the given binding index.
     * @param index the binding index
     */
    globjects::VertexAttributeBinding * attributeBinding(size_t index) const;

    /**
     * @brief associate an OpenGL buffer with a vertex attribute binding
     * @param bindingIndex the index of the vertex attribute binding
     * @param buffer the OpenGL buffer
     * @param baseOffset the base offset into the buffer for all vertices
     * @param stride difference in bytes between two adjacent verices
     */
    void setAttributeBindingBuffer(size_t bindingIndex, globjects::Buffer * buffer, gl::GLint baseOffset, gl::GLint stride);

    /**
     * @brief associates an OpenGL buffer (identified by the buffer index) with a vertex attribute binding
     * @param bindingIndex the index of the vertex attribute binding
     * @param bufferIndex the OpenGL buffer index (needs to be associated before this call using setBuffer)
     * @param baseOffset the base offset into the buffer for all vertices
     * @param stride difference in bytes between two adjacent verices
     */
    void setAttributeBindingBuffer(size_t bindingIndex, size_t bufferIndex, gl::GLint baseOffset, gl::GLint stride);

    /**
     * @brief configures the format of one vertex attribute (used for float inputs)
     * @param bindingIndex the index of the vertex attribute binding
     * @param size the number of components
     * @param type the data type of the source data
     * @param normalized a flag iff the data should be normalized before passed as input
     * @param relativeOffset the relative offset of this input to the start of the vertex in the associated buffer
     */
    void setAttributeBindingFormat(size_t bindingIndex, gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeOffset);

    /**
     * @brief configures the format of one vertex attribute (used for integer inputs)
     * @param bindingIndex the index of the vertex attribute binding
     * @param size the number of components
     * @param type the data type of the source data (should be an integer type)
     * @param relativeOffset the relative offset of this input to the start of the vertex in the associated buffer
     */
    void setAttributeBindingFormatI(size_t bindingIndex, gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset);

    /**
     * @brief configures the format of one vertex attribute (used for long integer inputs)
     * @param bindingIndex the index of the vertex attribute binding
     * @param size the number of components
     * @param type the data type of the source data (should be a long integer type)
     * @param relativeOffset the relative offset of this input to the start of the vertex in the associated buffer
     */
    void setAttributeBindingFormatL(size_t bindingIndex, gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset);

    /**
     * @brief associates a vertex attribute binding index with a vertex shader attribute input index.
     * @param bindingIndex the index of the vertex attribute binding
     * @param attributeIndex the index of the vertex shader attribute
     */
    void bindAttribute(size_t bindingIndex, gl::GLint attributeIndex);

    /**
     * @brief associates a collection of vertex attribute bindings to their vertex shader attribute counterparts.
     * @param attributeIndices the collection of associations where the vector index is the vertex attribute binding index and the values are the vertex shader attribute indices
     */
    void bindAttributes(const std::vector<gl::GLint> & attributeIndices);

    /**
     * @brief enable the vertex shader attribute associated with a vertex attribute binding
     * @param bindingIndex the index of the vertex attribute binding
     */
    void enableAttributeBinding(size_t bindingIndex);

    /**
     * @brief enable the vertex shader attributes associated by all configured vertex attribute bindings
     */
    void enableAllAttributeBindings();
protected:
    globjects::ref_ptr<globjects::VertexArray> m_vao; /// The VertexArray used for the vertex shader input specification and draw call triggering
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Buffer>> m_buffers; /// The collection of all buffers associated with this geometry. (Note: this class can be used without storing actual buffers here)
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Texture>> m_textures; /// The collection of all textures associated with this geometry. The key is used as the active texture binding.

    DrawMode m_drawMode; /// The configured draw mode that is used if no specific draw mode is passed in the draw method.
    gl::GLsizei m_size; /// The configured vertex count that is used if no specific vertex range is passed in the draw method.
    gl::GLenum m_mode; /// The configured primitive mode that is used if no specific primitive mode is passed in the draw method.
    gl::GLenum m_indexBufferType; /// The configured GPU index buffer type of the currently set index buffer.
    globjects::ref_ptr<globjects::Buffer> m_indexBuffer; /// The configured GPU index buffer that is used if no specific index buffer in passed in the draw method.
    std::vector<std::uint32_t> m_indices; /// The configured CPU index buffer that is used if no specific index buffer in passed in the draw method (Note: implied GL_UNSIGNED_INT as index buffer type).

protected:
    void bindTextures() const;
};


} // namespace gloperate


#include <gloperate/primitives/MyDrawable.hpp>
