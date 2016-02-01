
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

    void render(const GlyphVertexCloud & vertexCloud) const;

    globjects::Program * program();
    const globjects::Program * program() const;

protected:
    globjects::ref_ptr<globjects::Program> m_program;
};


} // namespace gloperate_text
