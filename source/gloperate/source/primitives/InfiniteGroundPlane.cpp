#include <gloperate/primitives/InfiniteGroundPlane.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>

#include <gloperate/primitives/VertexDrawable.h>

using namespace globjects;

namespace gloperate
{
const char * InfiniteGroundPlane::s_defaultVertexShaderSource = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec4 in_position;

uniform mat4 viewProjection;
uniform float height;

void main()
{
    gl_Position = viewProjection * in_position;
}
)";

const char* InfiniteGroundPlane::s_defaultFragmentShaderSource = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

void main()
{
}
)";

InfiniteGroundPlane::InfiniteGroundPlane()
: InfiniteGroundPlane(nullptr)
{
}

InfiniteGroundPlane::InfiniteGroundPlane(globjects::Shader * fragmentShader)
{
    StringTemplate* vertexShaderSource = new StringTemplate(new StaticStringSource(s_defaultVertexShaderSource));
    StringTemplate* fragmentShaderSource = new StringTemplate(new StaticStringSource(s_defaultFragmentShaderSource));

#ifdef __APPLE__
    vertexShaderSource->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif

    if (!fragmentShader)
    {
        fragmentShader = new Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource);
    }

    m_program = new globjects::Program;
    m_program->attach(new Shader(gl::GL_VERTEX_SHADER, vertexShaderSource), fragmentShader);

    // create infinite ground plane
    // see http://stackoverflow.com/questions/12965161/rendering-infinitely-large-plane y and z had to be flipped, the winding of the triangles also had to be changed
    glm::vec4 verts[] = { glm::vec4(0, 0, 0, 1), glm::vec4(1, 0, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(-1, 0, 0, 0), glm::vec4(0, 0, -1, 0) };

    m_drawable = new gloperate::VertexDrawable(
        std::vector<glm::vec4>({
            verts[0], verts[2], verts[1],
            verts[0], verts[3], verts[2],
            verts[0], verts[4], verts[3],
            verts[0], verts[1], verts[4]
        }),
        gl::GL_TRIANGLES);

    m_drawable->setFormats({ gloperate::Format(4, gl::GL_FLOAT, 0) });
    m_drawable->bindAttributes(m_program->getAttributeLocations({ "in_position" }));
    m_drawable->enableAll();
}

void InfiniteGroundPlane::draw(const glm::mat4 &viewProjectionMatrix)
{
    m_program->setUniform("viewProjection", viewProjectionMatrix);

    m_program->use();
    m_drawable->draw();
    m_program->release();
}

globjects::Program * InfiniteGroundPlane::program() const
{
    return m_program;
}

}
