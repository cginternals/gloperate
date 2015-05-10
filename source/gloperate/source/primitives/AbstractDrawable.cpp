
#include <globjects/VertexArray.h>

#include <gloperate/primitives/AbstractDrawable.h>


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
