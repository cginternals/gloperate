
#include <gloperate/stages/base/ShapeStage.h>

#include <glm/vec2.hpp>

#include <cppassist/memory/make_unique.h>

#include <gloperate/rendering/Point.h>
#include <gloperate/rendering/Quad.h>
#include <gloperate/rendering/Triangle.h>
#include <gloperate/rendering/Box.h>
#include <gloperate/rendering/Sphere.h>


using namespace cppassist;


namespace gloperate
{


CPPEXPOSE_COMPONENT(ShapeStage, gloperate::Stage)


ShapeStage::ShapeStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "ShapeStage", name)
, shapeType("shapeType", this)
, width("width", this, 2.0f)
, height("height", this, 2.0f)
, depth("depth", this, 2.0f)
, radius("radius", this, 1.0f)
, texCoords("texCoords", this, true)
, drawable("drawable", this)
{
}

ShapeStage::~ShapeStage()
{
}

void ShapeStage::onContextInit(gloperate::AbstractGLContext *)
{
}

void ShapeStage::onContextDeinit(AbstractGLContext *)
{
    // Clean up OpenGL objects
    m_shape = nullptr;
}

void ShapeStage::onProcess()
{
    // Get shape options
    ShapeType type = *shapeType;
    Flags<ShapeOption> options = ShapeOption::None;
    if (*this->texCoords) options = ShapeOption::IncludeTexCoords;

    // Create shape
    switch (type)
    {
        case ShapeType::Point:
            m_shape = cppassist::make_unique<Point>(options);
            break;

        case ShapeType::Quad:
            m_shape = cppassist::make_unique<Quad>(*this->width, *this->height, options);
            break;

        case ShapeType::Triangle:
            m_shape = cppassist::make_unique<Triangle>(*this->width, *this->height, options);
            break;

        case ShapeType::Box:
            m_shape = cppassist::make_unique<Box>(*this->width, *this->height, *this->depth, options);
            break;

        case ShapeType::Sphere:
            m_shape = cppassist::make_unique<Sphere>(*this->radius, options);
            break;

        default:
            m_shape = cppassist::make_unique<Shape>(options);
            break;
    }

    // Set output
    this->drawable.setValue(m_shape.get());
}


} // namespace gloperate
