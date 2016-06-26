
#pragma once

#include <vector>
#include <unordered_map>
#include <array>
#include <cstdint>

#include <glbinding/gl/types.h>

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/base/Referenced.h>
#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate_api.h>

#include <gloperate/primitives/AbstractDrawable.h>

namespace gloperate
{

/**
 * @brief
 *   The DrawMode enum
 *
 *   Used to specify the draw mode used in the Drawable class.
 */
enum class DrawMode : unsigned int
{
    Arrays, /// dispatches to glDrawArrays.
    ElementsIndices, /// dispatches to glDrawElements using a CPU index buffer.
    ElementsIndexBuffer /// dispatches to glDrawElements using a GPU index buffer.
};

/**
 * @brief
 *   The Drawable class wraps the setup and drawing routine for Vertex Arrays, Vertex Attribute Bindings and Buffers.
 *
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
class GLOPERATE_API Drawable : public globjects::Referenced, gloperate::AbstractDrawable
{
public:
    /**
     * @brief
     *   Constructor of Drawable
     *   Creates OpenGL objects, thus, a current context is required.
     */
    Drawable();

    /**
      * @brief
      *   Destructor
      */
    ~Drawable();

    /**
     * @brief
     *   A convenience accessor for the internal VertexArray instance.
     *
     * @return
     *   the VertexArray instance.
     */
    globjects::VertexArray * vao() const;

    /**
     * @brief
     *   The drawMode accessor.
     *
     * @return
     *   the current configured DrawMode.
     */
    DrawMode drawMode() const;

    /**
     * @brief
     *   Updates the configured DrawMode to distinguish the main intended OpenGL draw call.
     *
     * @param[in] drawMode
     *   The new draw mode.
     */
    void setDrawMode(DrawMode drawMode);

    /**
     * @brief
     *   The main draw entry point, used by the superclass AbstractDrawable.
     *
     *   Internally, this method dispatches to the other draw* methods based on the current drawMode.
     */
    virtual void draw() const override;

    /**
     * @brief
     *   A second entry point where a different drawMode than the currently configured can be used.
     *
     * @param[in] drawMode
     *   The drawMode to be used for dispatching this specific draw call.
     */
    void draw(DrawMode drawMode) const;

    /**
     * @brief
     *   The draw entry point for per vertex based drawing.
     *
     *   It triggers a glDrawArrays draw call with the currently configured mode using the full vertex buffers.
     */
    void drawArrays() const;

    /**
     * @brief
     *   Another draw entry point for per vertex based drawing where a different primitive mode than the currently configured can be used.
     *
     * @param[in] mode
     *   The primitive mode to be used for this specific draw call.
     */
    void drawArrays(gl::GLenum mode) const;

    /**
     * @brief
     *   Another draw entry point for per vertex based drawing where a different vertex buffer range than the currently configured can be used.
     *
     * @param[in] first
     *   The index of the first vertex to be used for this specific draw call.
     * @param[in] count
     *   The number of vertices to be used for this specific draw call.
     */
    void drawArrays(gl::GLint first, gl::GLsizei count) const;

    /**
     * @brief
     *   Another draw entry point for per vertex based drawing where a different primitive mode and vertex buffer range than the currently configured can be used.
     *
     * @param[in] mode
     *   The primitive mode to be used for this specific draw call.
     * @param[in] first
     *   The index of the first vertex to be used for this specific draw call.
     * @param[in] count
     *   The number of vertices to be used for this specific draw call.
     */
    void drawArrays(gl::GLenum mode, gl::GLint first, gl::GLsizei count) const;

    /**
     * @brief
     *   The draw entry point for index based drawing.
     *
     *   It triggers a glDrawElements draw call with the currently configured mode and index buffer (either CPU or GPU) using the full index range.
     */
    void drawElements() const;

    /**
     * @brief
     *   Another draw entry point for index based drawing where a different primitive mode than the currently configured can be used.
     *
     * @param[in] mode
     *   The primitive mode to be used for this specific draw call.
     */
    void drawElements(gl::GLenum mode) const;

    /**
     * @brief
     *   Another draw entry point for index based drawing where a different primitive mode than the currently configured and a specific CPU index buffer can be used.
     *
     * @param mode
     *   The primitive mode to be used for this specific draw call.
     * @param count
     *   The number of indices to use for this specific draw call.
     * @param type
     *   The data type of the indices to use for this specific draw call.
     * @param indices
     *   The pointer to the index buffer, containing adjacent values of the type passed in as type parameter.
     */
    void drawElements(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices) const;

    /**
     * @brief
     *   Another draw entry point for index based drawing where a different primitive mode than the currently configured and a specific GPU index buffer can be used.
     *
     * @param[in] mode
     *   The primitive mode to be used for this specific draw call.
     * @param[in] count
     *   The number of indices to use for this specific draw call.
     * @param[in] type
     *   The data type of the indices to use for this specific draw call.
     * @param[in] indices
     *   The OpenGL buffer containing the indices, which are adjacent values of the type passed in as type parameter.
     */
    void drawElements(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, globjects::Buffer * indices) const;

    /**
     * @brief
     *   Accessor for the configured vertex count of the geometry (usually that is the count parameter for the associated draw calls).
     *
     * @return
     *   The configured vertex count of the geometry.
     */
    gl::GLsizei size() const;

    /**
     * @brief
     *   Updates the configured vertex count of the geometry.
     *
     * @param[in] size
     *   The new size
     *
     * @remark
     *   This size have not to be equal to the maximum length of one or all associated buffers but is advised not to exceed the ranges of any buffer.
     */
    void setSize(gl::GLsizei size);

    /**
     * @brief
     *   The accessor for the currently configured draw call primitive mode.
     *
     * @return
     *   The currently configured draw call primitive mode.
     */
    gl::GLenum mode() const;

    /**
     * @brief
     *   Updates the configured primitive mode used in the draw calls.
     *
     * @param[in] mode
     *   The new primitive mode.
     */
    void setMode(gl::GLenum mode);

    /**
     * @brief
     *   Convenience method to initialize or update a vertex buffer.
     *
     * @tparam VectorType
     *   The typeof the vector elements
     * @param[in] index
     *   The index of the vertex buffer.
     * @param[in] data
     *   The new data.
     *
     * @remarks
     *   The indices don't need to be continuous.
     */
    template <typename VectorType>
    void setData(size_t index, const std::vector<VectorType> & data);

    /**
     * @brief
     *   Convenience method to initialize or update a vertex buffer.
     *
     * @tparam ArrayType
     *   The element type of the array.
     * @tparam ArraySize
     *   The element count of the array.
     * @param[in] index
     *   The index of the vertex buffer.
     * @param[in] data
     *   The new data.
     */
    template <typename ArrayType, size_t ArraySize>
    void setData(size_t index, const std::array<ArrayType, ArraySize> & data);

    /**
     * @brief
     *   Returns the OpenGL buffer at a given index.
     *
     * @param[in] index
     *   The index of the OpenGL buffer.
     *
     * @return
     *   The OpenGL buffer at the given index.
     *
     * @remarks
     *   The indices don't need to be continuous.
     *   If an OpenGL buffer at the given index doesn't exist a new one will be created.
     */
    globjects::Buffer * buffer(size_t index);

    /**
     * @brief
     *   Returns the OpenGL buffer at a given index.
     *
     * @param[in] index
     *   The index of the OpenGL buffer.
     *
     * @return
     *   The OpenGL buffer at the given index.
     *
     * @remarks
     *   The indices don't need to be continuous.
     *   If an OpenGL buffer at the given index doesn't exist a new one will be created.
     */
    globjects::Buffer * buffer(size_t index) const;

    /**
     * @brief
     *   Associates an OpenGL buffer with an index.
     *
     * @param[in] index
     *   The index of the OpenGL buffer.
     * @param[in] buffer
     *   The buffer to associate.
     *
     * @remarks
     *   The indices don't need to be continuous and one buffer can be assigned to more than one index.
     *   A buffer doesn't need to be associated with an index to be usable by this class. However if it is associated, the buffer object is partially owned by this object.
     */
    void setBuffer(size_t index, globjects::Buffer * buffer);

    /**
     * @brief
     *   Accessor for the index buffer.
     *
     * @return
     *   The index buffer.
     *
     * @remarks
     *   The return value may be a null pointer.
     */
    globjects::Buffer * indexBuffer() const;

    /**
     * @brief
     *   Updates the configured index buffer.
     *
     * @param[in] buffer
     *   The new OpenGL index buffer.
     */
    void setIndexBuffer(globjects::Buffer * buffer);

    /**
     * @brief
     *   Updates the configured index buffer and its type.
     *
     * @param[in] buffer
     *   The new OpenGL index buffer.
     * @param[in] bufferType
     *   The type of the indices.
     */
    void setIndexBuffer(globjects::Buffer * buffer, gl::GLenum bufferType);

    /**
     * @brief
     *   Accessor for the currently configured GPU index data type.
     *
     * @return
     *   The currently configured GPU index data type.
     */
    gl::GLenum indexBufferType() const;

    /**
     * @brief
     *   Updates the configured index data type.
     *
     * @param[in] bufferType
     *   The new data type.
     */
    void setIndexBufferType(gl::GLenum bufferType);

    /**
     * @brief
     *   Accessor for the currently configured CPU index buffer.
     *
     * @return
     *   The currently configured CPU index buffer.
     */
    const std::vector<std::uint32_t> & indices() const;

    /**
     * @brief
     *   Updates the configured CPU index buffer.
     *
     * @param[in] indices
     *   The vector of indices (needs to be of type GL_UNSIGNED_INT).
     */
    void setIndices(const std::vector<std::uint32_t> & indices);

    /**
     * @brief
     *   Accessor for a vertex attribute binding of the underlying vertex array object.
     *
     * @param[in] index
     *   The binding index.
     *
     * @return
     *   The vertex attribute binding at the given binding index.
     */
    globjects::VertexAttributeBinding * attributeBinding(size_t index) const;

    /**
     * @brief
     *   Associate an OpenGL buffer with a vertex attribute binding.
     *
     * @param[in] bindingIndex
     *   The index of the vertex attribute binding.
     * @param[in] buffer
     *   The OpenGL buffer.
     * @param[in] baseOffset
     *   The base offset into the buffer for all vertices.
     * @param[in] stride
     *   Difference in bytes between two adjacent verices.
     *
     */
    void setAttributeBindingBuffer(size_t bindingIndex, globjects::Buffer * buffer, gl::GLint baseOffset, gl::GLint stride);

    /**
     * @brief
     *   Associates an OpenGL buffer (identified by the buffer index) with a vertex attribute binding.
     *
     * @param[in] bindingIndex
     *   The index of the vertex attribute binding.
     * @param[in] bufferIndex
     *   The OpenGL buffer index (needs to be associated before this call using setBuffer).
     * @param[in] baseOffset
     *   The base offset into the buffer for all vertices.
     * @param[in] stride
     *   Difference in bytes between two adjacent verices.
     */
    void setAttributeBindingBuffer(size_t bindingIndex, size_t bufferIndex, gl::GLint baseOffset, gl::GLint stride);

    /**
     * @brief
     *   Configures the format of one vertex attribute (used for float inputs).
     *
     * @param[in] bindingIndex
     *   The index of the vertex attribute binding.
     * @param[in] size
     *   The number of components.
     * @param[in] type
     *   The data type of the source data.
     * @param[in] normalized
     *   A flag iff the data should be normalized before passed as input.
     * @param[in] relativeOffset
     *   The relative offset of this input to the start of the vertex in the associated buffer.
     */
    void setAttributeBindingFormat(size_t bindingIndex, gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeOffset);

    /**
     * @brief
     *   Configures the format of one vertex attribute (used for integer inputs).
     *
     * @param[in] bindingIndex
     *   The index of the vertex attribute binding.
     * @param[in] size
     *   The number of components.
     * @param[in] type
     *   The data type of the source data (should be an integer type).
     * @param[in] relativeOffset
     *   The relative offset of this input to the start of the vertex in the associated buffer.
     */
    void setAttributeBindingFormatI(size_t bindingIndex, gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset);

    /**
     * @brief
     *   Configures the format of one vertex attribute (used for long integer inputs).
     *
     * @param[in] bindingIndex
     *   The index of the vertex attribute binding.
     * @param[in] size
     *   The number of components.
     * @param[in] type
     *   The data type of the source data (should be a long integer type).
     * @param[in] relativeOffset
     *   The relative offset of this input to the start of the vertex in the associated buffer.
     */
    void setAttributeBindingFormatL(size_t bindingIndex, gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset);

    /**
     * @brief
     *   Associates a vertex attribute binding index with a vertex shader attribute input index.
     *
     * @param[in] bindingIndex
     *   The index of the vertex attribute binding.
     * @param[in] attributeIndex
     *   The index of the vertex shader attribute.
     */
    void bindAttribute(size_t bindingIndex, gl::GLint attributeIndex);

    /**
     * @brief
     *   Associates a collection of vertex attribute bindings to their vertex shader attribute counterparts.
     *
     * @param[in] attributeIndices
     *   The collection of associations where the vector index is the vertex attribute binding index and the values are the vertex shader attribute indices.
     */
    void bindAttributes(const std::vector<gl::GLint> & attributeIndices);

    /**
     * @brief
     *   Enable the vertex shader attribute associated with a vertex attribute binding.
     *
     * @param[in] bindingIndex
     *   The index of the vertex attribute binding.
     */
    void enableAttributeBinding(size_t bindingIndex);

    /**
     * @brief
     *   Enable the vertex shader attributes associated by all configured vertex attribute bindings.
     */
    void enableAllAttributeBindings();


protected:
    globjects::ref_ptr<globjects::VertexArray> m_vao; /// The VertexArray used for the vertex shader input specification and draw call triggering
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Buffer>> m_buffers; /// The collection of all buffers associated with this geometry. (Note: this class can be used without storing actual buffers here)

    DrawMode m_drawMode; /// The configured draw mode that is used if no specific draw mode is passed in the draw method.
    gl::GLsizei m_size; /// The configured vertex count that is used if no specific vertex range is passed in the draw method.
    gl::GLenum m_mode; /// The configured primitive mode that is used if no specific primitive mode is passed in the draw method.
    gl::GLenum m_indexBufferType; /// The configured GPU index buffer type of the currently set index buffer.
    globjects::ref_ptr<globjects::Buffer> m_indexBuffer; /// The configured GPU index buffer that is used if no specific index buffer in passed in the draw method.
    std::vector<std::uint32_t> m_indices; /// The configured CPU index buffer that is used if no specific index buffer in passed in the draw method (Note: implied GL_UNSIGNED_INT as index buffer type).
};


} // namespace gloperate


#include <gloperate/primitives/Drawable.hpp>
