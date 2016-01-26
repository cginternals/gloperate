#pragma once

#include <vector>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate/primitives/VertexDrawable.h>

#include <gloperate-text/gloperate-text_api.h>

#include <gloperate-text/geometry/GlyphVertex.h>


namespace gloperate_text
{


class GLOPERATE_TEXT_API GlyphVertexCloud
{
public:
    GlyphVertexCloud();

    void verticesChanged();

    gloperate::VertexDrawable * drawable();
    const gloperate::VertexDrawable * drawable() const;

public:
    std::vector<GlyphVertex> vertices;
    globjects::ref_ptr<globjects::Texture> glyphTexture;

protected:
    globjects::ref_ptr<gloperate::VertexDrawable> m_drawable;
};


} // namespace gloperate_text
