
#pragma once

#include <globjects/base/ref_ptr.h>
#include <globjects/Shader.h>
#include <globjects/Program.h>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{


class IndirectGlyphVertexCloud;

class GLOPERATE_TEXT_API IndirectGlyphRenderer
{
public:
    IndirectGlyphRenderer();
    IndirectGlyphRenderer(const globjects::Shader * fragmentShader);
    IndirectGlyphRenderer(const globjects::Program * program);
    virtual ~IndirectGlyphRenderer();

    void render(const IndirectGlyphVertexCloud & vertexCloud) const;

    globjects::Program * program();
    const globjects::Program * program() const;

protected:
    globjects::ref_ptr<globjects::Program> m_program;
};


} // namespace gloperate_text
