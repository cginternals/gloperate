
#include "DemoDrawableStage.h"

#include <glbinding/gl/gl.h>

#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>

#include <gloperate/rendering/Drawable.h>

#include <gloperate/gloperate.h>


// Geometry describing the triangle
static const std::array<glm::vec2, 4> s_vertices { {
    glm::vec2( +1.f, -1.f ),
    glm::vec2( +1.f, +1.f ),
    glm::vec2( -1.f, -1.f ),
    glm::vec2( -1.f, +1.f ) } };


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoDrawableStage, gloperate::Stage)


DemoDrawableStage::DemoDrawableStage(Environment * environment, const std::string & name)
: Stage(environment, "DemoDrawableStage", name)
, drawable("drawable", this, nullptr)
{
}

DemoDrawableStage::~DemoDrawableStage()
{
}

void DemoDrawableStage::onContextInit(AbstractGLContext *)
{
    m_drawable = cppassist::make_unique<Drawable>();
    m_drawable->setPrimitiveMode(gl::GL_TRIANGLE_STRIP);
    m_drawable->setDrawMode(DrawMode::Arrays);

    m_vertexBuffer = cppassist::make_unique<globjects::Buffer>();
    m_vertexBuffer->setData(s_vertices, gl::GL_STATIC_DRAW);

    m_drawable->setBuffer(0, m_vertexBuffer.get());

    m_drawable->setAttributeBindingBuffer(0, 0, 0, sizeof(glm::vec2));

    m_drawable->setAttributeBindingFormat(0, 2, gl::GL_FLOAT, gl::GL_FALSE, 0);

    m_drawable->enableAllAttributeBindings();

    m_drawable->setSize(s_vertices.size());
}

void DemoDrawableStage::onContextDeinit(AbstractGLContext *)
{
}

void DemoDrawableStage::onProcess(AbstractGLContext *)
{
    drawable.setValue(m_drawable.get());
}


} // namespace gloperate
