
#include <gloperate/primitives/AbstractDrawable.h>

#include <globjects/VertexArray.h>


namespace gloperate
{


AbstractDrawable::AbstractDrawable()
{
}

AbstractDrawable::~AbstractDrawable()
{
}

globjects::VertexArray * AbstractDrawable::vertexArray() const
{
    return m_vao.get();
}


} // namespace gloperate
