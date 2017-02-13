
#pragma once

#include <glm/fwd.hpp>

#include <globjects/base/ref_ptr.h>

#include <gloperate-text/gloperate-text_api.h>


namespace globjects
{
    class Shader;
    class Program;
}


namespace gloperate_text
{


class GlyphVertexCloud;


class GLOPERATE_TEXT_API GlyphRenderer
{
public:
    GlyphRenderer();
    explicit GlyphRenderer(globjects::Shader * fragmentShader);
    explicit GlyphRenderer(globjects::Program * program);
    virtual ~GlyphRenderer();

    GlyphRenderer & operator=(const GlyphRenderer &) = delete;

    globjects::Program * program();
    const globjects::Program * program() const;

    void render(const GlyphVertexCloud & vertexCloud) const;
    void renderInWorld(const GlyphVertexCloud & vertexCloud, const glm::mat4 & viewProjection) const;


protected:
    globjects::ref_ptr<globjects::Program> m_program;
};


} // namespace gloperate_text
