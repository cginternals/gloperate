
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

#include <gloperate/rendering/AbstractDrawable.h>


namespace gloperate
{


/**
*  @brief
*    Draw mode
*
*    Specifies the draw mode (e.g., indexed or arrays) for a Drawable.
*/
enum class DrawMode : unsigned int
{
    Arrays,             ///< Dispatch to glDrawArrays
    ElementsIndices,    ///< Dispatch to glDrawElements using a CPU index buffer
    ElementsIndexBuffer ///< Dispatch to glDrawElements using a GPU index buffer
};


/**
*  @brief
*    Drawable geometry
*
*    The Drawable class wraps the setup and drawing routine for vertex arrays,
*    vertex attribute bindings, and buffers. It can be used to define and render
*    geometries on the GPU.
*
*    Supported drawing types:
*    - glDrawArrays
*    - glDrawElements using CPU index buffer
*    - glDrawElements using GPU index buffer
*
*    Supported buffer arrangements:
*    - Separate buffer per vertex attribute
*    - Interleaved buffer for all vertex attributes
*    - Arbitrary distribution of vertex attributes to buffers (mixed separate and interleaved buffers)
*
*   Note: most configurable parameters (such as DrawMode, primitive mode, draw count,
*         index buffer source, ...) can be temporarily overwritten for each draw call.
*/
class GLOPERATE_API Drawable : public globjects::Referenced, gloperate::AbstractDrawable
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @remarks
    *    Creates OpenGL objects, thus, a current context is required.
    */
    Drawable();

    /**
    *  @brief
    *    Destructor
    *
    *  @remarks
    *    Destroys OpenGL objects, thus, a current context is required.
    */
    virtual ~Drawable();

    /**
    *  @brief
    *    Get vertex array object
    *
    *  @return
    *    Pointer to the vertex array object (always valid)
    */
    globjects::VertexArray * vao() const;

    /**
    *  @brief
    *    Get draw mode
    *
    *  @return
    *    Configured draw mode
    */
    DrawMode drawMode() const;

    /**
    *  @brief
    *    Set draw mode
    *
    *  @return
    *    Configured draw mode
    *
    *  @remarks
    *    The draw mode can be temporarilly overwritten by calling the
    *    specific draw-methods.
    */
    void setDrawMode(DrawMode drawMode);

    /**
    *  @brief
    *    Draw geometry
    *
    *  @remarks
    *    This function is the main draw entry point, used by the superclass
    *    AbstractDrawable. It dispatches to one of the other draw-methods
    *    based on the draw mode set by setDrawMode().
    */
    virtual void draw() const override;

    /**
    *  @brief
    *    Draw geometry with specific draw mode
    *
    *  @param[in] drawMode
    *    Draw mode to be used for dispatching this specific draw call
    */
    void draw(DrawMode drawMode) const;

    /**
    *  @brief
    *    Draw geometry by rendering full vertex buffers
    *
    *  @remarks
    *    Triggers a glDrawArrays draw call with the currently configured
    *    primitive mode using the full vertex buffers.
    */
    void drawArrays() const;

    /**
    *  @brief
    *    Draw geometry by rendering full vertex buffers, override primitive mode
    *
    *  @param[in] mode
    *    Primitive mode to be used for this specific draw call
    *
    *  @remarks
    *    Triggers a glDrawArrays draw call with the specified
    *    primitive mode using the full vertex buffers.
    */
    void drawArrays(gl::GLenum mode) const;

    /**
    *  @brief
    *    Draw geometry by rendering vertex buffers, override vertex buffer range
    *
    *  @param[in] first
    *    Index of the first vertex to be used for this specific draw call
    *  @param[in] count
    *    Number of vertices to be used for this specific draw call
    */
    void drawArrays(gl::GLint first, gl::GLsizei count) const;

    /**
    *  @brief
    *    Draw geometry by rendering vertex buffers, override primitive mode and vertex buffer range
    *
    *  @param[in] mode
    *    Primitive mode to be used for this specific draw call
    *  @param[in] first
    *    Index of the first vertex to be used for this specific draw call
    *  @param[in] count
    *    Number of vertices to be used for this specific draw call
    */
    void drawArrays(gl::GLenum mode, gl::GLint first, gl::GLsizei count) const;

    /**
    *  @brief
    *    Draw geometry by index-based rendering
    *
    *  @remarks
    *    Triggers a glDrawElements draw call with the currently configured
    *    primitive mode and index buffer (either CPU or GPU) using the full
    *    index range.
    */
    void drawElements() const;

    /**
    *  @brief
    *    Draw geometry by index-based rendering, override primitive mode
    *
    *  @param[in] mode
    *    Primitive mode to be used for this specific draw call
    */
    void drawElements(gl::GLenum mode) const;

    /**
    *  @brief
    *    Draw geometry by index-based rendering, override primitive mode and index buffer
    *
    *  @param[in] mode
    *    Primitive mode to be used for this specific draw call
    *  @param[in] count
    *    Number of indices to use for this specific draw call
    *  @param[in] type
    *    Data type of the indices to use for this specific draw call
    *  @param[in] indices
    *    Pointer to the index buffer (must NOT be nullptr), containing adjacent values of the type passed in as type parameter
    */
    void drawElements(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices) const;

    /**
    *  @brief
    *    Draw geometry by index-based rendering, override primitive mode and index buffer
    *
    *  @param[in] mode
    *    Primitive mode to be used for this specific draw call
    *  @param[in] count
    *    Number of indices to use for this specific draw call
    *  @param[in] type
    *    Data type of the indices to use for this specific draw call
    *  @param[in] indices
    *    OpenGL buffer containing the index buffer (must NOT be nullptr), containing adjacent values of the type passed in as type parameter
    */
    void drawElements(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, globjects::Buffer * indices) const;

    /**
    *  @brief
    *    Get vertex count
    *
    *  @return
    *    The configured vertex count of the geometry
    */
    gl::GLsizei size() const;

    /**
    *  @brief
    *    Set vertex count
    *
    *  @param[in] size
    *    The configured vertex count of the geometry
    *
    *  @remark
    *    This size can be smaller than the maximum length of one or all
    *    associated buffers but must not exceed the ranges of any buffer.
    */
    void setSize(gl::GLsizei size);

    /**
    *  @brief
    *    Get primitive mode
    *
    *  @return
    *    Current primitive mode for draw calls
    */
    gl::GLenum primitiveMode() const;

    /**
    *  @brief
    *    Set primitive mode
    *
    *  @param[in] mode
    *    Current primitive mode for draw calls
    */
    void setPrimitiveMode(gl::GLenum mode);

    /**
    *  @brief
    *    Set vertex buffer data
    *
    *  @tparam VectorType
    *    The type of the vector elements
    *  @param[in] index
    *    Vertex buffer index
    *  @param[in] data
    *    Vertex data
    *
    *  @remarks
    *    The indices don't need to be continuous.
    */
    template <typename VectorType>
    void setData(size_t index, const std::vector<VectorType> & data);

    /**
    *  @brief
    *    Set vertex buffer data
    *
    *  @tparam ArrayType
    *    The element type of the array
    *  @tparam ArraySize
    *    The element count of the array
    *  @param[in] index
    *    Vertex buffer index
    *  @param[in] data
    *    Vertex data
    *
    *  @remarks
    *    The indices don't need to be continuous.
    */
    template <typename ArrayType, size_t ArraySize>
    void setData(size_t index, const std::array<ArrayType, ArraySize> & data);

    /**
    *  @brief
    *    Get vertex buffer
    *
    *  @param[in] index
    *    Vertex buffer index
    *
    *  @return
    *    Vertex buffer (never null)
    *
    *  @remarks
    *    The indices don't need to be continuous.
    *    If an OpenGL buffer at the given index doesn't exist, a new one will be created.
    */
    globjects::Buffer * buffer(size_t index);

    /**
    *  @brief
    *    Get vertex buffer
    *
    *  @param[in] index
    *    Vertex buffer index
    *
    *  @return
    *    Vertex buffer (can be null)
    *
    *  @remarks
    *    The indices don't need to be continuous.
    */
    globjects::Buffer * buffer(size_t index) const;

    /**
    *  @brief
    *    Set vertex buffer
    *
    *  @param[in] index
    *    Vertex buffer index
    *  @param[in] buffer
    *    Vertex buffer
    *
    *  @remarks
    *    The indices don't need to be continuous. One buffer can be assigned to more than
    *    one index. A buffer doesn't need to be associated with an index to be usable by
    *    this class. However, if it is associated, the buffer object is partially owned by
    *    this object.
    */
    void setBuffer(size_t index, globjects::Buffer * buffer);

    /**
    *  @brief
    *    Get index buffer
    *
    *  @return
    *    Index buffer (can be null)
    */
    globjects::Buffer * indexBuffer() const;

    /**
    *  @brief
    *    Set index buffer
    *
    *  @param[in] buffer
    *    Index buffer
    */
    void setIndexBuffer(globjects::Buffer * buffer);

    /**
    *  @brief
    *    Set index buffer and type
    *
    *  @param[in] buffer
    *    Index buffer
    *  @param[in] bufferType
    *    Type of the indices
    */
    void setIndexBuffer(globjects::Buffer * buffer, gl::GLenum bufferType);

    /**
    *  @brief
    *    Get index buffer type
    *
    *  @return
    *    Type of the indices
    */
    gl::GLenum indexBufferType() const;

    /**
    *  @brief
    *    Set index buffer type
    *
    *  @param[in] bufferType
    *    Type of the indices
    */
    void setIndexBufferType(gl::GLenum bufferType);

    /**
    *  @brief
    *    Get index buffer data
    *
    *  @return
    *    CPU index buffer
    */
    const std::vector<std::uint32_t> & indices() const;

    /**
    *  @brief
    *    Set index buffer data
    *
    *  @param[in] indices
    *    CPU index buffer (needs to be of type GL_UNSIGNED_INT).
    */
    void setIndices(const std::vector<std::uint32_t> & indices);

    /**
    *  @brief
    *    Get vertex attribute binding
    *
    *  @param[in] index
    *    Binding index
    *
    *  @return
    *    Vertex attribute binding
    */
    globjects::VertexAttributeBinding * attributeBinding(size_t index) const;

    /**
    *  @brief
    *    Set vertex attribute binding
    *
    *  @param[in] bindingIndex
    *    Binding index
    *  @param[in] bufferIndex
    *    Buffer index (see setBuffer)
    *  @param[in] baseOffset
    *    The base offset into the buffer for all vertices
    *  @param[in] stride
    *    Difference in bytes between two adjacent vertices
    */
    void setAttributeBindingBuffer(size_t bindingIndex, size_t bufferIndex, gl::GLint baseOffset, gl::GLint stride);

    /**
    *  @brief
    *    Set format for vertex attribute binding (used for float input)
    *
    *  @param[in] bindingIndex
    *    Binding index
    *  @param[in] size
    *    Number of components
    *  @param[in] type
    *    Data type of the source data
    *  @param[in] normalized
    *    A flag specifying wether the data should be normalized before passed as input
    *  @param[in] relativeOffset
    *    The relative offset of this input to the start of the vertex in the associated buffer
    */
    void setAttributeBindingFormat(size_t bindingIndex, gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeOffset);

    /**
    *  @brief
    *    Set format for vertex attribute binding (used for integer input)
    *
    *  @param[in] bindingIndex
    *    Binding index
    *  @param[in] size
    *    Number of components
    *  @param[in] type
    *    Data type of the source data
    *  @param[in] relativeOffset
    *    The relative offset of this input to the start of the vertex in the associated buffer
    */
    void setAttributeBindingFormatI(size_t bindingIndex, gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset);

    /**
    *  @brief
    *    Set format for vertex attribute binding (used for long integer input)
    *
    *  @param[in] bindingIndex
    *    Binding index
    *  @param[in] size
    *    Number of components
    *  @param[in] type
    *    Data type of the source data
    *  @param[in] relativeOffset
    *    The relative offset of this input to the start of the vertex in the associated buffer
    */
    void setAttributeBindingFormatL(size_t bindingIndex, gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset);

    /**
    *  @brief
    *    Associate vertex attribute binding with a vertex shader attribute input index
    *
    *  @param[in] bindingIndex
    *    Index of the vertex attribute binding
    *  @param[in] attributeIndex
    *    Index of the vertex shader attribute
    */
    void bindAttribute(size_t bindingIndex, gl::GLint attributeIndex);

    /**
    *  @brief
    *    Associate vertex attribute bindings with their vertex shader attribute counterparts
    *
    *  @param[in] attributeIndices
    *    Collection of associations where the vector index is the vertex attribute binding index and the values are the vertex shader attribute indices
    */
    void bindAttributes(const std::vector<gl::GLint> & attributeIndices);

    /**
    *  @brief
    *    Enable vertex shader attribute associated with a vertex attribute binding
    *
    *  @param[in] bindingIndex
    *    Index of the vertex attribute binding
    */
    void enableAttributeBinding(size_t bindingIndex);

    /**
    * @brief
    *   Enable vertex shader attributes associated with all configured vertex attribute bindings
    */
    void enableAllAttributeBindings();


protected:
    globjects::ref_ptr<globjects::VertexArray>                        m_vao;     ///< The VertexArray used for the vertex shader input specification and draw call triggering
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Buffer>> m_buffers; ///< The collection of all buffers associated with this geometry. (Note: this class can be used without storing actual buffers here)

    DrawMode                              m_drawMode;        ///< The configured draw mode that is used if no specific draw mode is passed in the draw method.
    gl::GLsizei                           m_size;            ///< The configured vertex count that is used if no specific vertex range is passed in the draw method.
    gl::GLenum                            m_primitiveMode;   ///< The configured primitive mode that is used if no specific primitive mode is passed in the draw method.
    gl::GLenum                            m_indexBufferType; ///< The configured GPU index buffer type of the currently set index buffer.
    globjects::ref_ptr<globjects::Buffer> m_indexBuffer;     ///< The configured GPU index buffer that is used if no specific index buffer in passed in the draw method.
    std::vector<std::uint32_t>            m_indices;         ///< The configured CPU index buffer that is used if no specific index buffer in passed in the draw method (Note: implied GL_UNSIGNED_INT as index buffer type).
};


} // namespace gloperate


#include <gloperate/rendering/Drawable.inl>
