
#include <gloperate-text/GlyphRenderer.h>

#include <glbinding/gl/enum.h>

#include <globjects/base/File.h>
#include <globjects/Shader.h>
#include <globjects/Program.h>

#include <gloperate/gloperate.h>

#include <gloperate-text/GlyphVertexCloud.h>


namespace gloperate_text
{


GlyphRenderer::GlyphRenderer()
: GlyphRenderer(new globjects::Shader(gl::GL_FRAGMENT_SHADER
    , new globjects::File(gloperate::dataPath()+"/gloperate-text/shaders/glyph.frag")))
{
}

GlyphRenderer::GlyphRenderer(globjects::Shader * fragmentShader)
: GlyphRenderer(new globjects::Program)
{
    m_program->attach(new globjects::Shader(gl::GL_VERTEX_SHADER
        , new globjects::File(gloperate::dataPath()+"/gloperate-text/shaders/glyph.vert")));
    m_program->attach(new globjects::Shader(gl::GL_GEOMETRY_SHADER
        , new globjects::File(gloperate::dataPath()+"/gloperate-text/shaders/glyph.geom")));
    m_program->attach(fragmentShader);

    m_program->setUniform<gl::GLint>("glyphs", 0);
}

GlyphRenderer::GlyphRenderer(globjects::Program * program)
: m_program(program)
{
    m_program->setUniform<gl::GLint>("glyphs", 0);
}

GlyphRenderer::~GlyphRenderer()
{
}

void GlyphRenderer::render(const GlyphVertexCloud & vertexCloud) const
{
    if (vertexCloud.vertices().empty())
    {
        return;
    }

    m_program->use();

    vertexCloud.texture()->bindActive(0);
    vertexCloud.drawable()->draw();
    vertexCloud.texture()->unbindActive(0);

    m_program->release();
}

globjects::Program * GlyphRenderer::program()
{
    return m_program;
}

const globjects::Program * GlyphRenderer::program() const
{
    return m_program;
}


} // namespace
