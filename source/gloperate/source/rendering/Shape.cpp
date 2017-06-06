
#include <gloperate/rendering/Shape.h>


namespace gloperate
{


Shape::Shape(ShapeType shapeType, cppassist::Flags<ShapeOption> options)
: m_type(shapeType)
, m_options(options)
{
}

Shape::~Shape()
{
}

ShapeType Shape::type() const
{
    return m_type;
}

cppassist::Flags<ShapeOption> Shape::options() const
{
    return m_options;
}


} // namespace gloperate
