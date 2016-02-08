
#include <gloperate/primitives/MyDrawable.h>

#include <cassert>

#include <glbinding/gl/enum.h>

#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>


namespace gloperate
{


MyDrawable::MyDrawable()
: m_vao(new globjects::VertexArray)
, m_drawMode(DrawMode::Arrays)
, m_size(0)
, m_mode(gl::GL_TRIANGLES)
, m_indexBufferType(gl::GL_UNSIGNED_INT)
{
}

MyDrawable::~MyDrawable()
{
}

globjects::VertexArray * MyDrawable::vao() const
{
    return m_vao;
}

DrawMode MyDrawable::drawMode() const
{
    return m_drawMode;
}

void MyDrawable::setDrawMode(DrawMode drawMode)
{
    m_drawMode = drawMode;
}

void MyDrawable::draw() const
{
    draw(m_drawMode);
}

void MyDrawable::draw(DrawMode drawMode) const
{
    switch (drawMode)
    {
    case DrawMode::ElementsIndices:
    case DrawMode::ElementsIndexBuffer:
        drawElements();
        break;
    case DrawMode::Arrays:
    default:
        drawArrays();
        break;
    }
}

void MyDrawable::drawArrays() const
{
    drawArrays(m_mode, 0, m_size);
}

void MyDrawable::drawArrays(gl::GLenum mode) const
{
    drawArrays(mode, 0, m_size);
}

void MyDrawable::drawArrays(gl::GLint first, gl::GLsizei count) const
{
    drawArrays(m_mode, first, count);
}

void MyDrawable::drawArrays(gl::GLenum mode, gl::GLint first, gl::GLsizei count) const
{
    bindTextures();
    m_vao->drawArrays(mode, first, count);
}

void MyDrawable::drawElements() const
{
    drawElements(m_mode);
}

void MyDrawable::drawElements(gl::GLenum mode) const
{
    if (m_drawMode == DrawMode::ElementsIndices)
    {
        drawElements(mode, m_size, gl::GL_UNSIGNED_INT, m_indices.data());
    }
    else
    {
        drawElements(mode, m_size, m_indexBufferType, m_indexBuffer);
    }
}

void MyDrawable::drawElements(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices) const
{
    globjects::Buffer::unbind(gl::GL_ELEMENT_ARRAY_BUFFER);
    bindTextures();
    m_vao->drawElements(mode, count, type, indices);
}

void MyDrawable::drawElements(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, globjects::Buffer * indices) const
{
    indices->bind(gl::GL_ELEMENT_ARRAY_BUFFER);
    bindTextures();
    m_vao->drawElements(mode, count, type, nullptr);
}

gl::GLsizei MyDrawable::size() const
{
    return m_size;
}

void MyDrawable::setSize(gl::GLsizei size)
{
    m_size = size;
}

gl::GLenum MyDrawable::mode() const
{
    return m_mode;
}

void MyDrawable::setMode(gl::GLenum mode)
{
    m_mode = mode;
}

globjects::Buffer * MyDrawable::buffer(size_t index)
{
    if (m_buffers.count(index) == 0)
    {
        setBuffer(index, new globjects::Buffer);
    }

    return m_buffers.at(index);
}

globjects::Buffer * MyDrawable::buffer(size_t index) const
{
    return m_buffers.at(index);
}

void MyDrawable::setBuffer(size_t index, globjects::Buffer * buffer)
{
    m_buffers.emplace(index, buffer);
}

globjects::Texture * MyDrawable::texture(size_t index)
{
    if (m_textures.count(index) == 0)
    {
        setTexture(index, new globjects::Texture);
    }

    return m_textures.at(index);
}

globjects::Texture * MyDrawable::texture(size_t index) const
{
    return m_textures.at(index);
}

globjects::Texture * MyDrawable::texture(gl::GLenum activeTextureIndex)
{
    assert(activeTextureIndex >= gl::GL_TEXTURE0);

    return texture(static_cast<size_t>(activeTextureIndex) - static_cast<size_t>(gl::GL_TEXTURE0));
}

globjects::Texture * MyDrawable::texture(gl::GLenum activeTextureIndex) const
{
    assert(activeTextureIndex >= gl::GL_TEXTURE0);

    return texture(static_cast<size_t>(activeTextureIndex) - static_cast<size_t>(gl::GL_TEXTURE0));
}

void MyDrawable::setTexture(size_t index, globjects::Texture * texture)
{
    m_textures.emplace(index, texture);
}

void MyDrawable::setTexture(gl::GLenum activeTextureIndex, globjects::Texture * texture)
{
    assert(activeTextureIndex >= gl::GL_TEXTURE0);

    return setTexture(static_cast<size_t>(activeTextureIndex) - static_cast<size_t>(gl::GL_TEXTURE0), texture);
}

globjects::Texture * MyDrawable::removeTexture(size_t index)
{
    if (m_textures.count(index) == 0)
    {
        return nullptr;
    }

    globjects::Texture * texture = m_textures.at(index);

    m_textures.erase(index);

    return texture;
}

globjects::Texture * MyDrawable::removeTexture(gl::GLenum activeTextureIndex)
{
    return removeTexture(static_cast<size_t>(activeTextureIndex) - static_cast<size_t>(gl::GL_TEXTURE0));
}

globjects::Buffer * MyDrawable::indexBuffer() const
{
    return m_indexBuffer;
}

void MyDrawable::setIndexBuffer(globjects::Buffer * buffer)
{
    m_indexBuffer = buffer;
}

void MyDrawable::setIndexBuffer(globjects::Buffer * buffer, gl::GLenum bufferType)
{
    setIndexBuffer(buffer);
    setIndexBufferType(bufferType);
}

gl::GLenum MyDrawable::indexBufferType() const
{
    return m_indexBufferType;
}

void MyDrawable::setIndexBufferType(gl::GLenum bufferType)
{
    m_indexBufferType = bufferType;
}

const std::vector<std::uint32_t> & MyDrawable::indices() const
{
    return m_indices;
}

void MyDrawable::setIndices(const std::vector<std::uint32_t> & indices)
{
    m_indices = indices;
}

globjects::VertexAttributeBinding * MyDrawable::attributeBinding(size_t index) const
{
    return m_vao->binding(index);
}

void MyDrawable::setAttributeBindingBuffer(size_t bindingIndex, globjects::Buffer * buffer, gl::GLint baseOffset, gl::GLint stride)
{
    m_vao->binding(bindingIndex)->setBuffer(buffer, baseOffset, stride);
}

void MyDrawable::setAttributeBindingBuffer(size_t bindingIndex, size_t bufferIndex, gl::GLint baseOffset, gl::GLint stride)
{
    assert(m_buffers.count(bufferIndex) > 0);

    m_vao->binding(bindingIndex)->setBuffer(m_buffers.at(bufferIndex), baseOffset, stride);
}

void MyDrawable::setAttributeBindingFormat(size_t bindingIndex, gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeOffset)
{
    m_vao->binding(bindingIndex)->setFormat(size, type, normalized, relativeOffset);
}

void MyDrawable::setAttributeBindingFormatI(size_t bindingIndex, gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset)
{
    m_vao->binding(bindingIndex)->setIFormat(size, type, relativeOffset);
}

void MyDrawable::setAttributeBindingFormatL(size_t bindingIndex, gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset)
{
    m_vao->binding(bindingIndex)->setLFormat(size, type, relativeOffset);
}

void MyDrawable::bindAttribute(size_t bindingIndex, gl::GLint attributeIndex)
{
    m_vao->binding(bindingIndex)->setAttribute(attributeIndex);
}

void MyDrawable::bindAttributes(const std::vector<gl::GLint> & attributeIndices)
{
    for (size_t i = 0; i < attributeIndices.size(); ++i)
    {
        m_vao->binding(i)->setAttribute(attributeIndices.at(i));
    }
}

void MyDrawable::enableAttributeBinding(size_t bindingIndex)
{
    m_vao->enable(m_vao->binding(bindingIndex)->attributeIndex());
}

void MyDrawable::enableAllAttributeBindings()
{
    for (const globjects::VertexAttributeBinding * binding : m_vao->bindings())
    {
        m_vao->enable(binding->attributeIndex());
    }
}

void MyDrawable::bindTextures() const
{
    for (const auto & pair : m_textures)
    {
        if (pair.second == nullptr)
        {
            continue;
        }

        pair.second->bindActive(pair.first);
    }
}


} // namespace gloperate
