
#pragma once

#include <globjects/base/ref_ptr.h>
#include <globjects/Shader.h>
#include <globjects/Program.h>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{


class GlyphVertexCloud;


class GLOPERATE_TEXT_API GlyphRenderer
{

public:

    GlyphRenderer();
    GlyphRenderer(globjects::Shader * fragmentShader);
    GlyphRenderer(globjects::Program * program);
    virtual ~GlyphRenderer();

    globjects::Program * program();
    const globjects::Program * program() const;

    void render(const GlyphVertexCloud & vertexCloud) const;
    void renderInWorld(const GlyphVertexCloud & vertexCloud, const glm::mat4 & viewProjection) const;

protected:

    globjects::ref_ptr<globjects::Program> m_program;
};


} // namespace gloperate_text
